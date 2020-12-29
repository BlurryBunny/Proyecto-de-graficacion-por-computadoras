/*----------------- Clase Arista----------------*/
#ifndef EDGE_H
#define EDGE_H


class Edge{
private:
	
public:
	Vertex vi;
	Vertex vf;
	int id;
	Edge(Vertex vi, Vertex vf, int id);
	Edge();
  void imprime_Arista();
  int getID();
  void setID(int idnuevo);
	Vertex getVI();
	Vertex getVF();
};

#endif
