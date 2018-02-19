/*
 *
 * Breadth First Search (BFS)
 *
 */

#pragma once

#include <app/graph.h>

#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>


template<class T>
class SafeQueue {
public:

    void queue(const T &element) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(element);
    }

    T deque() {
        std::lock_guard<std::mutex> lock(m_mutex);
        T val;
        if (!m_queue.empty()) {
            val = m_queue.front();
            m_queue.pop();
        }

        return val;
    }

    const bool empty() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    const size_t size() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

private:
    std::mutex              m_mutex;
    std::queue<T>           m_queue;
};

typedef SafeQueue<NodeRef> NodeQueue;


class VisitVector {
public:

	VisitVector(const size_t size)
		: m_vector(std::vector<std::atomic_bool>(size))
	{
		
	}

	void set(const size_t index, const bool &val) {
		m_vector[index].store(val);
	}

	const bool read(const size_t index) const {
		return m_vector[index].load();
	}

	const bool empty() const {
		return m_vector.empty();
	}

	const size_t size() const {
		return m_vector.size();
	}

private:
	std::mutex						m_mutex;
	std::vector<std::atomic_bool>   m_vector;
};

NodeRef findT(NodeQueue &toProcess, const std::string& id, VisitVector &visitedNodes, const size_t size, std::atomic_int &checked, std::atomic_bool &stop)
{
	while (!stop) {
		auto node = toProcess.deque();
		if (!node) continue;

		checked++;
		if (node->getId() == id) {
			stop.store(true);
			return node;
		}

		if (checked >= size) {
			stop.store(true);
			return nullptr;
		}

		auto connections = node->getConnections();
		for (auto itr = connections.begin(); itr != connections.end(); ++itr) {
			if (auto lockedNode = (*itr).lock()) {
				// skip the node if its already visited else queue it up
				if (!visitedNodes.read(lockedNode->index())) {
					visitedNodes.set(lockedNode->index(), true);
					toProcess.queue(lockedNode);
				}
			}
		}
	}

	return nullptr;
}



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
            if (!graph || graph->empty()) {
                Log::error("Graph empty...skipping search");
                return nullptr;
            }

            auto rootNode = graph->getFirst();
            if (rootNode->getId() == id) return rootNode;

			NodeQueue processQueue;
			//NodeQueue visitedNodes;
			VisitVector visitedNodes(graph->size());
			
			std::atomic_int checked{0};
			std::atomic_bool stop{false};

			visitedNodes.set(0, true);
			processQueue.queue(rootNode);

			std::vector<std::future<NodeRef>> futures;

			for (size_t i = 0; i < 3; ++i)
				futures.emplace_back(std::async(findT, std::ref(processQueue), std::ref(id), std::ref(visitedNodes), graph->size(), std::ref(checked), std::ref(stop)));

			NodeRef result = nullptr;
			for (auto itr = futures.begin(); itr != futures.end(); ++itr) {
				auto res = (*itr).get();
				if (res) {
					result = res;
					break;
				}
			}	

			return result;
        }

};
