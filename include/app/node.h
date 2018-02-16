/*
 *
 * Node
 *
 */

#pragma once

#include <auxiliary/logger.h>
#include <auxiliary/test.h>

#include <memory>
#include <string>
#include <vector>

class Node;
typedef std::shared_ptr<Node> NodeRef;
typedef std::weak_ptr<Node> NodeWRef;

/**
 *
 * Node
 *
 * This class implements a graph node element.
 *
 */
class Node {

    private:
		/**
		 *
		 * Constructor
		 *
		 */
        Node() = delete;

    public:
		/**
		 *
		 * Constructor
		 *
		 */
        Node(const std::string& id) {
            m_id = id;
        }

    public:
		/**
		 *
		 * Factory method
		 *
		 * @param id Identifier of node
		 * @return Returns a reference to the created node instance
		 *
		 */
        static NodeRef createInstance(const std::string& id) {
            return std::make_shared<Node>(id);
        }

    public:
		/**
		 *
		 * Create new node instance and add it as a child node
		 *
		 * @param otherNode Reference to node to connect to
		 *
		 */
        void connect(NodeRef otherNode) {
            m_connections.push_back(otherNode);
        }

		/**
		 *
		 * Get node identifier
		 *
		 * @return Returns the node identifier
		 *
		 */
        std::string getId() const {
            return m_id;

        }

		/**
		 *
		 * Get node connections
		 *
		 * @return Returns the connections of the node to other nodes
		 *
		 */
        const std::vector<NodeWRef>& getConnections() const {
            return m_connections;
        }

		/**
		 *
		 * Check if the content of nodes are equal
		 *
		 * This function checks if the content (id) of nodes are equal.
		 *
		 * @return Returns true if equal, false otherwise.
		 *
		 */
		bool equals(NodeRef node) const {
            return node->getId() == m_id;
		}

        /**
         *
         * Set if the node was visited by a traverser
         *
         * @param visited flag to set visited or not
         *
         */
        void markVisited(const bool visited = true) {
            m_visited = visited;
        }

        /**
         * Checks if the node was visited by a traverser
         *
         * @return true if it was visited false otherwise
         *
         */
        const bool visited() const {
            return m_visited;
        }

    private:
        std::string                m_id{""};
        std::vector<NodeWRef>      m_connections;
        bool                       m_visited{false};

};
