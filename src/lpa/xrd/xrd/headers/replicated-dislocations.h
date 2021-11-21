int index_all=-1;
int ii,jj;
int idislo=0;

for (ii=-D_REPLICATION; ii<=D_REPLICATION; ii++) {
  for (jj=-D_REPLICATION; jj<=D_REPLICATION; jj++) {
    for (idislo=0; idislo< Nd0 ; idislo++) {
      index_all = index_all + 1;
      rd0_all[index_all].x = rd0[idislo].x + ii*size;
      rd0_all[index_all].y = rd0[idislo].y + jj*size;
      rd0_all[index_all].z = (double)(sd0[idislo]);
    }
  }
}
