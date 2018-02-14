/*
 *
 * Graph
 *
 */

#pragma once

#include <auxiliary/logger.h>
#include <auxiliary/test.h>

#include <app/node.h>

#include <memory>
#include <string>

class Graph;
typedef std::shared_ptr<Graph> GraphRef;

/**
 *
 * Graph
 *
 * This class implements a graph.
 *
 */
class Graph {

    public:
		/**
		 *
		 * Constructor
		 *
		 */
        Graph() { 
			clear(); 
		}

    public:
		/**
		 *
		 * Factory method
		 *
		 * @return Returns a reference to the created graph instance
		 *
		 */
        static GraphRef createInstance() {

            return std::make_shared<Graph>();

        }

    public:
		/**
		 *
		 * Create new node instance and add it as a child node
		 *
		 * @param id Identifier of node
		 * @return Returns reference to created node instance
		 *
		 */
        NodeRef addNode(const std::string& id) {

			// @TODO: Implement
			return nullptr;

        }

		/**
		 *
		 * Create an edge between two nodes
		 *
		 * @param node1 Reference to node
		 * @param node2 Reference to node
		 *
		 */
        void addEdge(NodeRef node1, NodeRef node2) {

			// @TODO: Implement

        }

		/**
		 *
		 * Get graph size
		 *
		 * This method returns the graph size as the number of nodes.
		 *
		 * @return Returns the graph size, or zero for an empty graph.
		 *
		 */
		size_t size() const {

			// @TODO: Implement
			return 0;

        }

		/**
		 *
		 * Clear graph
		 *
		 */
		void clear() {

			// @TODO: Implement

		}

		/**
		 *
		 * Check if graph is empty
		 *
		 * @return Returns true if the graph is empty, false otherwise.
		 *
		 */
		bool empty() const {

			// @TODO: Implement
			return true;

		}

		/**
		 *
		 * Get node with a specific index
		 *
		 * @param index Index of the node to be retrieved.
		 * @return Returns a reference to an existing node of the graph,
		 * or null if the index is out of range.
		 *
		 */
		NodeRef getNode(size_t index) const {

			// @TODO: Implement
			return nullptr;

		}

		/**
		 *
		 * Get first node of graph (root)
		 *
		 * @return Returns a reference to the first node of the graph,
		 * or null if the graph is empty.
		 *
		 */
		NodeRef getFirst() const {

			// @TODO: Implement
			return nullptr;

		}

};
