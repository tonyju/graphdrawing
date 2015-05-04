#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/cluster/ClusterOrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <iostream>
#include <fstream>
#include <string>
#include "csvparser.h"
#include <ogdf/energybased/FMMMLayout.h>
using namespace ogdf;

int main()
{
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	Graph G2;
	GraphAttributes GA2(G2);
	if (!G2.readGML("C:\\Users\\tonyju\\Documents\\Visual Studio 2013\\Projects\\graphdrawing\\graphdrawing\\test.gml")) {
		cerr << "Could not load test.gml" << endl;
		return 1;
	}
	node v;
	forall_nodes(v, G2)
		GA2.width(v) = GA2.height(v) = 10.0;

	FMMMLayout fmmm;

	fmmm.useHighLevelOptions(true);
	fmmm.unitEdgeLength(15.0);
	fmmm.newInitialPlacement(true);
	fmmm.qualityVersusSpeed(FMMMLayout::qvsGorgeousAndEfficient);

	fmmm.call(GA2);
	GA2.writeSVG("C:\\Users\\tonyju\\Documents\\Visual Studio 2013\\Projects\\graphdrawing\\graphdrawing\\result.svg");
	/************************************************************************/
	/* test csv                                                                     */
	/************************************************************************/
	
	ifstream infile("C:\\Users\\tonyju\\Documents\\Visual Studio 2013\\Projects\\graphdrawing\\graphdrawing\\ieee123topo.csv");
	ofstream outfile("test1.gml");
	if (infile.bad())
	{
		std::cout << "bad!"<< std::endl;
		return 0;
	}
	std::map<int, int> nodemap;
	std::map<int, int> ::const_iterator p;
	std::string sLine;
	//std::string sCol1, sCol2, sCol4;
	//double iCol1,;
	int iCol1, iCol2;
	int nnode = 0;
	int nfr = 0;
	int nto = 0;
	CSVParser parser;
	Graph G1;
	GraphAttributes GA1(G1, GraphAttributes::nodeGraphics |
		GraphAttributes::edgeGraphics);
	while (!infile.eof()) {
		std::getline(infile, sLine); // Get a line
		if (sLine == "")
			continue;

		parser << sLine; // Feed the line to the parser

		// Now extract the columns from the line
		parser >> iCol1 >> iCol2;
		node tnode1,tnode2;
		node node1, node2;
		if (nodemap.count(iCol1))
		{
			p=nodemap.find(iCol1);
			nfr = (*p).second;
		}
		else
		{
			//node [ id 0 ]
			nnode++;
			nodemap.insert(std::pair<int, int>(iCol1, nnode));
			nfr = nnode;
			outfile << "  node [ id" << " " << nfr << " ]" << std::endl;
		}
		if (nodemap.count(iCol2))
		{
			p = nodemap.find(iCol2);
			nto = (*p).second;
		}
		else
		{
			nnode++;
			nodemap.insert(std::pair<int, int>(iCol2, nnode));
			nto = nnode;
			outfile << "  node [ id" << " " << nto << " ]" << std::endl;
		}
		//edge [ source 1 target 13 ]
		outfile << "edge [ source " << nfr << " target " << nto << " ]" << std::endl;
		//p.pushBack(DPoint(10, -20 ));
		//p.pushBack(DPoint(20 , -10));
		//std::cout << "Column1: " << iCol1 << endl
		//	<< "Column2: " << iCol2 << endl
		//	<< endl;
	}
	infile.close();
	outfile.close();



	/************************************************************************/
	/* end of test csv                                                                     */
	/************************************************************************/
	
	
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
		//p.pushBack(DPoint(10, -20 * i));
		//p.pushBack(DPoint(20 * (LEN - i), -10));
	}
	PlanarizationLayout pl;
	GA1.writeSVG("C:\\Users\\tonyju\\Documents\\Visual Studio 2013\\Projects\\graphdrawing\\graphdrawing\\er-diagram-before.svg");
	system("iexplore C:\\Users\\tonyju\\Documents\\Visual Studio 2013\\Projects\\graphdrawing\\graphdrawing\\er-diagram-before.svg");
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
	ol->separation(200.0);
	ol->cOverhang(0.4);
	ol->setOptions(2 + 4);
	pl.setPlanarLayouter(ol);
	
	//pl.call(GA);
	//GA.writeSVG("C:\\Users\\tonyju\\Documents\\Visual Studio 2013\\Projects\\graphdrawing\\graphdrawing\\er-diagram-layoutsvg.svg");
	pl.call(GA1);
	GA1.writeSVG("C:\\Users\\tonyju\\Documents\\Visual Studio 2013\\Projects\\graphdrawing\\graphdrawing\\er-diagram-layoutsvg.svg");
	system("iexplore C:\\Users\\tonyju\\Documents\\Visual Studio 2013\\Projects\\graphdrawing\\graphdrawing\\er-diagram-layoutsvg.svg");
	//GA.writeGML("er-diagram-layout.gml");

	return 0;
}