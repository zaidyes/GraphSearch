/*
 *
 * Node
 *
 */

#pragma once

#include <auxiliary/logger.h>
#include <auxiliary/test.h>

#include <atomic>
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
        Node(const std::string& id, const size_t &index) {
            m_id = id;
			m_index = index;
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
        static NodeRef createInstance(const std::string& id, const size_t &index) {
            return std::make_shared<Node>(id, index);
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

		size_t index() const { return m_index; }

    private:
        std::string                m_id{""};
        std::vector<NodeWRef>      m_connections;
		size_t					   m_index{0};

};
