/*
 *
 * Breadth First Search (BFS)
 *
 */

#pragma once

#include <app/graph.h>

#include <string>

/**
 * 
 *  Breadth-first-search (BFS) implementation
 * 
 */
class BreadthFirstSearch {
    
    public:
        /**
         * 
         * Find a named node.
         * 
         * This method performs a BFS and returns the first node with
         * a given identifier.
         * 
         * @param root Root node to start the search.
         * @param id Identifier to be found.
         * @return Returns the found node, or null in case no node
         * has been found.
         * 
         */
        NodeRef find(GraphRef graph, const std::string& id) {
            if (!graph) {
                Log::error("Graph invalid...skipping search");
                return nullptr;
            }

            std::cout << "id:" << id << std::endl;

            // The vector holds our visited nodes
            //std::vector<NodeRef> visitedNodes;
            //visitedNodes.reserve(graph->size());

            std::queue<NodeRef> toProcess;

            auto rootNode = graph->getFirst();
            //visitedNodes.push_back(rootNode);
            toProcess.push(rootNode);

            while(!toProcess.empty()) {

                auto node = toProcess.front();
                toProcess.pop();
                //Log::infof("now processing id: %d", node->getId());

                if (node->getId() == id) {
                    std::cout << "found id:" << id << std::endl;
                    return node;
                }

                auto connections = node->getConnections();
                for(auto itr = connections.begin(); itr != connections.end(); ++itr) {
                    if (auto lockedNode = (*itr).lock()) {
                        /*auto visitedItr = std::find(visitedNodes.begin(), visitedNodes.end(), lockedNode);
                        // skip the node if its already visited else queue it up
                        if (visitedItr == visitedNodes.end()) {
                            visitedNodes.push_back(lockedNode);
                            toProcess.push(lockedNode);
                        }*/
                        toProcess.push(lockedNode);
                    }
                }

            }

            std::cout << "not found id:" << id << std::endl;


			return nullptr;

        }

};
