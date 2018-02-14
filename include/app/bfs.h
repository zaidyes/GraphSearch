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

			// @TODO: Implement
			return nullptr;

        }

};
