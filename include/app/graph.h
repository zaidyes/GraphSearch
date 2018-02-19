/*
 *
 * Graph
 *
 */

#pragma once

#include <auxiliary/logger.h>
#include <auxiliary/test.h>

#include <app/node.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

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
            Log::info("Graph created");
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
            auto newNode = Node::createInstance(id, m_nodeMap.size());
            m_nodeMap.push_back(newNode);
            return newNode;


            /* ////TO ENABLE DUPLICATE ID CHECK USE THIS CODE///
            // check if we already have a node with this id.
            auto foundNode = std::find_if(m_nodeMap.begin(), m_nodeMap.end(), [&id](const NodeRef& node) {
                return node->getId() == id;
            });

            // if we already have node with this id return null else create a new node
            if (foundNode != m_nodeMap.end()) {
                Log::errorf("Node with id: %d already exists", id);
                return nullptr;
            } else {
                auto newNode = Node::createInstance(id);
                m_nodeMap.push_back(newNode);
                Log::debugf("new node created succesfully, id: %d", id);
                return newNode;
            }
            */
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
            if (!node1 || !node2) {
                Log::error("cannot add edge ebcause node is invalid");
                return;
            }

            Log::debug("Successfully added edge");
            node1->connect(node2);
            node2->connect(node1);
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
            return m_nodeMap.size();
        }

		/**
		 *
		 * Clear graph
		 *
		 */
		void clear() {
            m_nodeMap.clear();
		}

		/**
		 *
		 * Check if graph is empty
		 *
		 * @return Returns true if the graph is empty, false otherwise.
		 *
		 */
		bool empty() const {
            return m_nodeMap.empty();
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
            if (index >= m_nodeMap.size()) {
                Log::warn("Index out of bounds for node.");
                return nullptr;
            } else {
                return m_nodeMap[index];
            }
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
            if (m_nodeMap.empty()) {
                Log::error("Graph is empty");
                return nullptr;
            } else {
                return m_nodeMap[0];
            }
		}

private:
        std::vector<NodeRef> m_nodeMap;

};
