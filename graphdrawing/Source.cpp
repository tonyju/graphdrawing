#include <stdlib.h>
#include <stdio.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/cluster/ClusterOrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>


using namespace ogdf;

int main()
{
	Graph G;
	GraphAttributes GA(G, GraphAttributes::nodeGraphics |
		GraphAttributes::edgeGraphics);

	const int LEN = 11;
	for (int i = 1; i<LEN; ++i) {
		node left = G.newNode();
		GA.x(left) = -5;
		GA.y(left) = -20;
		GA.width(left) = 10;
		GA.height(left) = 15;

		node bottom = G.newNode();
		GA.x(bottom) = 20;
		GA.y(bottom) = 5;
		GA.width(bottom) = 15;
		GA.height(bottom) = 10;

		edge e = G.newEdge(left, bottom);
		DPolyline &p = GA.bends(e);
		p.pushBack(DPoint(10, -20 * i));
		p.pushBack(DPoint(20 * (LEN - i), -10));
	}
	PlanarizationLayout pl;
	GA.writeSVG("er-diagram-before.svg");

	FastPlanarSubgraph *ps = new FastPlanarSubgraph;
	ps->runs(100);
	VariableEmbeddingInserter *ves = new VariableEmbeddingInserter;
	ves->removeReinsert(EdgeInsertionModule::rrAll);
	pl.setSubgraph(ps);
	pl.setInserter(ves);

	EmbedderMinDepthMaxFaceLayers *emb = new EmbedderMinDepthMaxFaceLayers;
	pl.setEmbedder(emb);

	//OrthoLayout *ol = new OrthoLayout;
	OrthoLayout *ol = new OrthoLayout;
	ol->separation(20.0);
	ol->cOverhang(0.4);
	ol->setOptions(2 + 4);
	pl.setPlanarLayouter(ol);
	pl.call(GA);
	GA.writeSVG("er-diagram-layoutsvg.svg");
	system("iexplore C:\\Users\\tonyju\\Downloads\\ogdf.v2012.07\\OGDF\\er-diagram-layoutsvg.svg");
	//GA.writeGML("er-diagram-layout.gml");

	return 0;
}