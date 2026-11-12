inline unsigned int Segundos_a_Pulsos(float segundos) {
  return floor( segundos * 1000 );
}

inline int Pulsos_a_Segundos(unsigned int pulsos) {
  return floor( pulsos / 1000 );
}
