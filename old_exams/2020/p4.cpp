void evolveOneDay (
  const int *row_ptr, const int *col_idx, const int num_persons, const int T,
  const double f, const char *health_status
) {
  int self_idx, other_idx, j;
  char self_health, other_health;
  bool self_contagious, self_susceptible, self_able_to_influence;
  bool other_contagious, other_susceptible, other_able_to_influence;

  for (self_idx=0; self_idx<num_persons-1; self_idx++) {
    self_health = health_status[self_idx];
    self_contagious = ((0 < self_health) && (self_health < T+1));
    self_susceptible = (self_health == 0);
    self_able_to_influence = (self_contagious || self_susceptible)

    if !(self_able_to_influence)
      continue;

    for (j=row_ptr[i]; j<row_ptr[i+1]; j++) {
      other_idx = col_idx[j];

      other_health = health_status[other_idx];
      other_contagious = ((0 < other_health) && (other_health < T+1));
      other_susceptible = (other_health == 0);
      other_able_to_influence = (other_contagious || other_susceptible)

      if !(other_able_to_influence)
        continue;

      // move along if noone is contagious
      if !(self_contagious || other_contagious)
        continue;

      // move along if both are contagious
      if (self_contagious && other_contagious)
        continue;

      // generate a random number between 0 and 1 and compare to f
      infection = (rand(0, 1) < f);
      if !(infection)
        continue;

      // if self becomes infected, break the inner loop
      if (self_susceptible && other_contagious) {
        health_next_day[self_idx] = T+1;
        break;
      }

      if (self_contagious && other_susceptible)
        health_next_day[other_idx] = T+1;
    }
  }

  for (i=0; i<num_persons; i++) {
    if (health_status[i] > 1)
      health_status[i]--;
    else if (health_status[i] == 1)
      health_status[i] = -1;
  }
}
