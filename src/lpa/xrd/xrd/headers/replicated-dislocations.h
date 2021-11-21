int index_all = -1;

for (i=-D_REPLICATION; i<=D_REPLICATION; i++) {
  for (j=-D_REPLICATION; j<=D_REPLICATION; j++) {
    for (k=0; k<Nd0; k++) {
      index_all += 1;
      rd0_all[index_all].x = rd0[k].x + i*size;
      rd0_all[index_all].y = rd0[k].y + j*size;
      rd0_all[index_all].z = (double)(sd0[k]);
    }
  }
}
