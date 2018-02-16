/**
 *
 * Application
 *
 */

#pragma once

#include <app/bfs.h>

#include <auxiliary/logger.h>
#include <auxiliary/test.h>

#include <memory>
#include <cassert>

/*
 *
 * TEST PARAMETERS
 * 
 * RUN FINAL TEST WITH DEFAULT VALUES.
 * 
 */
static const int NUM_DATASET_LEVELS = 5; ///< Depth of the graph (default: 5)
static const int NUM_DATASET_NODES  = 5; ///< Number of child nodes per node (default: 5)

/**
 *
 * Breadth-First-Search Application
 *
 */
class Application {
	
	public:
		Application() { ; }

	public:

		/**
		 *
		 * Run application
		 *
		 * @return Returns 0 is search was successful, -1 in case of error.
		 *
		 */
		int run() {

			auto bfs = std::make_unique<BreadthFirstSearch>();
			auto graph = createGraph(NUM_DATASET_LEVELS, NUM_DATASET_NODES);
            bool status = performSearch(bfs, graph);
		
			return (status ? 1 : -1);
		}

	private:
		/**
		 * 
		 * Create child nodes.
		 * 
		 * This method recursively creates child nodes of a node.
		 * 
		 * @param root Root node to create childs for
		 * @param numNodes Number of child nodes per parent node to create
		 * @param numLevels Depth of the recursion to create child nodes
		 * @param level Internal level counter
		 * @param idx Internal node index counter
		 * 
		 * IMPORTANT: This method is not thread-safe!
		 * 
		 */
		void createChilds(GraphRef graph, NodeRef parent, int numNodes, int numLevels, int level) {
		
			char nameBuffer[64];
		
			for (int i=0; i<numNodes; i++) {
		
				sprintf(nameBuffer, "Node%d", (int) graph->size()-1);
		
				auto child = graph->addNode(nameBuffer);

				if (nullptr != parent) {
					graph->addEdge(parent, child);
				}
		
				if (level  < numLevels)
				{
					createChilds(graph, child, numNodes, numLevels, level+1);
				}
			}
		
		}
		
		/**
		 * 
		 * Create dataset
		 * 
		 * This method creates a graph with a given depth and given
		 * number of childs per parent node
		 * 
		 * @param levels Number of hierarchy levels (depth) of the graph
		 * @param nodes Number of nodes per parent node
		 * @return Returns a reference to the root node of the graph
		 * 
		 * IMPORTANT: This method is not thread-safe!
		 * 
		 */
		GraphRef createGraph(int levels, int nodes) {
		
			Log::infof("creating dataset...");

			auto graph = GraphRef(new Graph());

			graph->addNode("root");
		
			for (int level=0; level<levels; level++) {
				createChilds(graph, graph->getFirst(), nodes, levels, 0);
			}

			size_t numNodes = graph->size();
			size_t numEdges = 0;

			for (size_t edgeDistance = 2; edgeDistance < numNodes/2; edgeDistance *= 2) {

				for (size_t index = 0; index < (numNodes - edgeDistance*2); index += edgeDistance*2) {
					NodeRef node1 = graph->getNode(index);
					assert(nullptr != node1);

					NodeRef node2 = graph->getNode(index + edgeDistance);
					assert(nullptr != node1);

					graph->addEdge(node1, node2);
					numEdges++;
				
				}
			}
		
			Log::infof("created dataset with %d nodes, %d cross-level edges", (int) numNodes, (int) numEdges);
		
			return graph;
		}
		
		/**
		 * 
		 * Perform search
		 * 
		 * This method performs multiple breadth-first-searches on a
		 * given graph.
		 * 
		 * @param bfs Reference to breadth first search implementation
		 * @param graph Reference to a given graph
		 * @return Returns true if all search operations worked as expected,
		 * false otherwise.
		 * 
		 */
		bool performSearch(std::unique_ptr<BreadthFirstSearch>& bfs, GraphRef graph) {
		
			int absNodeCount = (int) graph->size();
			int notFoundNodeCount = 0;
		
			{
				Log::info("Searching existing...");
		
				auto tStart = std::chrono::high_resolution_clock::now();
		
				char nameBuffer[64];

				int percent = -1;

				for (int node = 0; node < absNodeCount; node += 23) {
					sprintf(nameBuffer, "Node%d", node);

					int nextPercent = (((node+1)*100)/absNodeCount);
					if (nextPercent != percent) {
						percent = nextPercent;
						printf("\r%d%%", percent);
						fflush (stdout);
					}

					if (nullptr == bfs->find(graph, nameBuffer)) {
						printf("\nnot found: %s\n", nameBuffer);
						bfs->find(graph, nameBuffer);
						notFoundNodeCount++;
					}
				}

				printf("\r             \r");
		
				auto tElapsed = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - tStart).count();
				Log::infof("Search existing - elapsed time = %0.3f seconds", tElapsed);
		
				if (notFoundNodeCount > 0) {
					Log::errorf("%d nodes have not been found!", notFoundNodeCount);
					return false;
				}
			}
		
			{
				Log::info("Searching for non-existing...");
		
				auto tStart = std::chrono::high_resolution_clock::now();
				auto result = bfs->find(graph, "DOES_NOT_EXIST");
				auto tElapsed = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - tStart).count();

				Log::infof("Search for non-existing time = %0.3f seconds", tElapsed);
				if (nullptr != result) {
					Log::error("non-existing nodes have been found!");
					return false;
				}					
					
			}
		
			return true;
		}

};

IMPLEMENT_TEST(minimalisticTest) {
	
	// create BFS object
	auto bfs = std::make_unique<BreadthFirstSearch>();
	testAssert(nullptr != bfs);

	// create graph
	auto graph = Graph::createInstance();
	testAssert(nullptr != graph);

	// check if everything is clean at the beginning
	testAssert(graph->empty());
	testAssert(graph->size() == 0);
	testAssert(nullptr == graph->getNode(0));
	testAssert(nullptr == graph->getFirst());

	// add nodes
	auto a = graph->addNode("A");
	auto b = graph->addNode("B");
	auto c = graph->addNode("C");
	auto d = graph->addNode("D");
	auto e = graph->addNode("E");
	auto f = graph->addNode("F");
	auto g = graph->addNode("G");
	auto h = graph->addNode("H");

	// check if the graph is filled
	testAssert(!graph->empty());
	testAssert(graph->size() == 8);

	auto graphFirst = graph->getFirst();					// get first element
	testAssert(nullptr != graphFirst);						// found an element
	testAssert(0 == graphFirst->getId().compare("A"));		// really element "A"
	auto graph0 = graph->getNode(0);						// get element 0
	testAssert(nullptr != graph0);							// found an element
	testAssert(0 == graph0->getId().compare("A"));			// really element "A"
	testAssert(graphFirst.get() == graph0.get());			// point to the same object

	graph->addEdge(a, d);									// add edges ...
	testAssert(a->getConnections().size() == 1);			// ... and check connections
	testAssert(d->getConnections().size() == 1);

	graph->addEdge(a, e);
	testAssert(a->getConnections().size() == 2);
	testAssert(e->getConnections().size() == 1);

	graph->addEdge(c, f);
	testAssert(c->getConnections().size() == 1);
	testAssert(f->getConnections().size() == 1);

	graph->addEdge(c, g);
	testAssert(c->getConnections().size() == 2);
	testAssert(g->getConnections().size() == 1);

	graph->addEdge(e, h);
	testAssert(e->getConnections().size() == 2);
	testAssert(h->getConnections().size() == 1);

	graph->addEdge(a, g);
	testAssert(a->getConnections().size() == 3);
	testAssert(g->getConnections().size() == 2);

	graph->addEdge(f, e);
	testAssert(f->getConnections().size() == 2);
	testAssert(e->getConnections().size() == 3);

	graph->addEdge(b, g);
	testAssert(b->getConnections().size() == 1);
	testAssert(g->getConnections().size() == 3);

	// check if node is found
	auto result = bfs->find(graph, "H");					
	testAssert(nullptr != result);

	// check if found node is the correct one
	if (nullptr != result) {
		testAssert(0 == h->getId().compare(result->getId()));
	}

	// check if non-existing node is not found
	testAssert(nullptr == bfs->find(graph, "DOES_NOT_EXIST"));

	// check cleanup
	graph->clear();
	testAssert(graph->empty());
	testAssert(graph->size() == 0);
	testAssert(nullptr == graph->getNode(0));
	testAssert(nullptr == graph->getFirst());

}

IMPLEMENT_TEST(VisitedUnsetTest) {

    // create BFS object
    auto bfs = std::make_unique<BreadthFirstSearch>();
    testAssert(nullptr != bfs);

    // create graph
    auto graph = Graph::createInstance();
    testAssert(nullptr != graph);

    // add nodes
    auto a = graph->addNode("A");
    auto b = graph->addNode("B");
    auto c = graph->addNode("C");

    testAssertFalse(a->visited());
    testAssertFalse(b->visited());
    testAssertFalse(c->visited());

    graph->addEdge(a, b);
    graph->addEdge(b, c);
    graph->addEdge(c, a);

    // check if node is not found
    bfs->find(graph, "DOES_NOT_EXIST");

    testAssertFalse(a->visited());
    testAssertFalse(b->visited());
    testAssertFalse(c->visited());
}

IMPLEMENT_TEST(cyclicSearchTest) {

    // create BFS object
    auto bfs = std::make_unique<BreadthFirstSearch>();
    testAssert(nullptr != bfs);

    // create graph
    auto graph = Graph::createInstance();
    testAssert(nullptr != graph);

    // add nodes
    auto a = graph->addNode("A");
    auto b = graph->addNode("B");
    auto c = graph->addNode("C");

    graph->addEdge(a, b);
    graph->addEdge(b, c);
    graph->addEdge(c, a);

    // check if node is not found
    auto result = bfs->find(graph, "DOES_NOT_EXIST");
    testAssert(nullptr == result);
}
