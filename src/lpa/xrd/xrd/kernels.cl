#pragma OPENCL EXTENSION cl_khr_fp64 : enable

/*
  A kernel is the algorithm expression for one work-item. Here, the work-items
  are processes that produce results relative to a random point (displacement
  field, Fourier transform).

  The first kernel is used to calculate the displacement field at the positions
  of the random points. The second kernel calculates the first harmonics of the
  Fourier transform for a given value of the Fourier variable.

  The displacement field is expressed in the dislocation frame. The direction
  of L (Fourier variable) and the diffraction vector are also expressed in the
  latter. It would be necessary to express the relative position of the
  dislocation in this reference frame, but this can be avoided by considering
  that the random points are expressed in the dislocation reference frame.
*/

__kernel void udislo(
  __global double *random1, // random numbers 1
  __global double *random2, // random numbers 2
  __global double3 *dislocations, // dislocation positions and Burgers vector senses
  __global double2 *r_rdm, // random points positions (crystal frame)
  __global double3 *u_rdm, // displacement field at random points (dislocation frame)
  const double be_len, // Burgers vector edge part norm [nm]
  const double bs_len, // Burgers vector screw part norm [nm]
  const double size, // radius or side of the region of interest [nm]
  const double nu, // Poisson's number
  const int Np, // number of random points
  const int Nd, // number of dislocations
  const int Flag_Square, // geometry of the region of interest
  __local double3 *shared // bunch of dislocations stored on shared memory
) {

  int k = get_global_id(0); // global work-item ID (index of the random point, from 0 to Np-1)
  int n_local = get_local_size(0); // number of local work-items in the work-group (bunch of random points)
  int i_local = get_local_id(0); // local work-item ID (from 0 to n_local-1)

  double2 r_k = (double2)(0.0f); // position of random point k (crystal frame)
  double3 u_k = (double3)(0.0f); // displacement field at random point k (dislocation frame)

  double uaux = 1.0f/(4.0f*(1.0f-nu));
  double uaux1 = 1.0f/(2.0f*(1-nu));
  double uaux2 = 1.0f - 2.0f*nu;
  double pi = 3.1415926535897f;

  double x; // relative x coordinate of a dislocation with repect to the random point (crystal frame)
  double y; // relative y coordiante of a dislocation with repect to the random point (crystal frame)
  double theta; // relative angular coordinate of a dislocation with repect to the random point (crystal frame)
  double x2; // x^2
  double y2; // y^2
  double x2_y2; // x^2 + y^2
  double sense; // dislocation sense

  if (k < Np) { // check that the work-item is assigned to an existing random point

    // calculate the random point position

    if (Flag_Square == 1) {
      r_k.x = random1[k] * size;
      r_k.y = random2[k] * size;
    } else {
      double r = random1[k] * size;
      double phi = random2[k] * 2.0f * pi;
      r_k.x = r * cos(phi);
      r_k.y = r * sin(phi);
    }

    // import and calculate the contribution of the dislocations by groups

    for (int i=0; i<Nd; i+=n_local) { // group by group

      // transfer simultaneously the dislocations of the group from host to shared memory

      if ((i+n_local) > Nd) {
        n_local = Nd - i;
      }

      if ((i+i_local) < Nd) {
        shared[i_local] = dislocations[i+i_local];
      }

      barrier(CLK_LOCAL_MEM_FENCE); // synchronization

      // compute the contribution of the dislocations located in the shared memory

      for(int j=0; j<n_local; j++) { // browse the dislocations in the shared memory
        x = r_k.x - shared[j].x; // crystal frame
        y = r_k.y - shared[j].y; // crystal frame
        sense = shared[j].z;
        x2 = x * x;
        y2 = y * y;
        x2_y2 = x2 + y2;
        theta = atan2(y, x);
        u_k.x += be_len * sense * (theta + x*y/x2_y2*uaux1);
        u_k.y -= be_len * sense * uaux * (uaux2*log(x2_y2) + (x2-y2)/x2_y2);
        u_k.z += bs_len * sense * theta;
      }

      barrier(CLK_LOCAL_MEM_FENCE); // synchronization
    }

    u_rdm[k] = u_k;
    r_rdm[k] = r_k;
  }
}

__kernel void comptf(
  __global double16 *Vect16FC, // data structure to transmit the results of the Fourier transform
  __global double3 *dislocations, // dislocation positions and Burgers vector senses
  __global double2 *r_rdm, // random points positions (crystal frame)
  __global double3 *u_rdm, // displacement field at random points (dislocation frame)
  const double be_len, // Burgers vector edge part norm [nm]
  const double bs_len, // Burgers vector screw part norm [nm]
  double size, // radius or side of the region of interest [nm]
  double nu, // Poisson's number
  const int Np, // number of random points
  const int i_L, // step multiplier of the Fourier variable (L = i_L * a3)
  const double g_vec_len, // difraction vector norm
  const double3 gd_vec, // difraction vector (dislocation frame)
  const double2 a3vd, // step multiplier vector (dislocation frame)
  const double a3, // step multiplier
  const int Nd, // number of dislocations
  const int Flag_Square, // geometry of the region of interest
  __global int *inout, // dislocations inside the region of interest
  __local double3 *shared // bunch of dislocations stored on shared memory
) {

  int k = get_global_id(0); // global work-item ID (index of the random point, from 0 to Np-1)
  int n_local = get_local_size(0); // number of local work-items in the work-group (bunch of random points)
  int i_local = get_local_id(0); // local work-item ID (from 0 to n_local-1)

  double uaux = 1.0f/(4.0f*(1.0f-nu));
  double uaux1 = 1.0f/(2.0f*(1-nu));
  double uaux2 = 1.0f-2.0f*nu;

  double3 u_k = u_rdm[k]; // displacement field at random point k (dislocation frame)
  double2 r_k = r_rdm[k]; // position of random point k (crystal frame)
  double3 u_k_translated = (double3)(0.0f); // displacement field translated position of random point k (dislocation frame)
  double2 r_k_translated; // translated position of random point k (crystal frame)
  int opposed_translation = 0; // first translation outside the region of interest but second inside
  double2 translation = (double2)(0.0f); // translation of the random point k position: i_L * a3
  double3 du = (double3)(0.0f); // displacement field difference
  double g_du_2pi; // phase
  double eps2; // mean square strain

  double x; // relative x coordinate of a dislocation with repect to the translated random point (crystal frame)
  double y; // relative y coordiante of a dislocation with repect to the translated random point (crystal frame)
  double theta; // relative angular coordinate of a dislocation with repect to the translated random point (crystal frame)
  double x2; // x^2
  double y2; // y^2
  double x2_y2; // x^2 + y^2
  double sense; // dislocation sense

  if (k < Np) { // check that the work-item is assigned to an existing random point

    // compute the translated position

    translation = (i_L+1) * a3vd;

    r_k_translated = r_k + translation;

    if (Flag_Square == 0) {
      if (length(r_k_translated) > size) {
        r_k_translated = r_k - translation;
        if (length(r_k_translated) > size) {
          inout[k] = 0; // translated point outside the region of interest
        } else {
          opposed_translation = 1;
        }
      }
    } else {
      if ((r_k_translated.x<0) || (r_k_translated.x>size) || (r_k_translated.y<0) || (r_k_translated.y>size)) {
        r_k_translated = r_k - translation;
        if ((r_k_translated.x<0) || (r_k_translated.x>size) || (r_k_translated.y<0) || (r_k_translated.y>size)) {
          inout[k] = 0; // translated point outside the region of interest
        } else {
          opposed_translation = 1;
        }
      }
    }

    // import and calculate the contribution of the dislocations by groups

    for (int i=0; i<Nd; i+=n_local) {

      // transfer simultaneously the dislocations of the group from host to shared memory

      if ((i+n_local) > Nd) {
        n_local = Nd - i;
      }

      if ((i+i_local) < Nd) {
        shared[i_local] = dislocations[i+i_local];
      }

      barrier(CLK_LOCAL_MEM_FENCE); // synchronization

      // compute the contribution of the dislocations located in the shared memory

      for(int j=0; j<n_local; j++) { // browse the dislocations in the shared memory
        x = r_k_translated.x - shared[j].x; // crystal frame
        y = r_k_translated.y - shared[j].y; // crystal frame
        sense = shared[j].z;
        x2 = x * x;
        y2 = y * y;
        x2_y2 = x2 + y2;
        theta = atan2(y, x);
        u_k_translated.x += be_len * sense * (theta + x*y/x2_y2*uaux1);
        u_k_translated.y -= be_len * sense * uaux * (uaux2*log(x2_y2) + (x2-y2)/x2_y2);
        u_k_translated.z += bs_len * sense * theta;
      }

      barrier(CLK_LOCAL_MEM_FENCE); // synchronization
    }

    // compute displacement field difference

    du = (opposed_translation == 1) ? u_k_translated-u_k : u_k-u_k_translated;

    // compute Fourier transform

    g_du_2pi = dot(gd_vec, du);

    eps2 = g_du_2pi/g_vec_len/((i_L+1)*a3);
    eps2 *= eps2;

    Vect16FC[k] = inout[k]*(double16)(
      cos(1.0f*g_du_2pi), sin(1.0f*g_du_2pi),
      cos(2.0f*g_du_2pi), sin(2.0f*g_du_2pi),
      cos(3.0f*g_du_2pi), sin(3.0f*g_du_2pi),
      cos(4.0f*g_du_2pi), sin(4.0f*g_du_2pi),
      cos(5.0f*g_du_2pi), sin(5.0f*g_du_2pi),
      eps2, 0.0, 0.0, 0.0, 0.0, 0.0);
  }
}
