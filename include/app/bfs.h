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

class ThreadedBFS {

public:

    static NodeRef findT(NodeQueue &toProcess, const std::string& id, NodeQueue &visitedNodes, const size_t size, std::atomic_int &checked, std::atomic_bool &stop)
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
                    if (!lockedNode->visited()) {
                        lockedNode->markVisited();
                        visitedNodes.queue(lockedNode);
                        toProcess.queue(lockedNode);
                    }
                }
            }
        }

        return nullptr;
    }

    void startThreads() {
        //std::cout << "Starting thread handling" << std::endl;

    }

public:

    void setSearchId(const std::string &id) {
        m_searchId = id;
    }

    void setRootNode(const NodeRef &rootNode) {
        m_rootNode = rootNode;
    }

    void setTotalNodeCount(const size_t totalNodes) {
        m_totalNodes = totalNodes;
    }

    void setMaxThreadCount(const int maxThreads) {
        m_maxThreadCount = maxThreads;
    }

    void stop() {
        m_stop = true;
    }

    bool startMTSearch() {
        if (!m_rootNode)
            return false;

        m_rootNode->markVisited();
        m_visitedNodes.queue(m_rootNode);
        m_processQueue.queue(m_rootNode);
        m_stop = false;

        // start atleast one thread to perform search
        m_futures.emplace_back(std::async(std::launch::async, findT, std::ref(m_processQueue), std::ref(m_searchId), std::ref(m_visitedNodes), m_totalNodes, std::ref(m_checked),  std::ref(m_stop)));

        // start more if required and when it is possible
        while (m_futures.size() < m_maxThreadCount) {
            //std::cout << "Starting new thread" << std::endl;
            m_futures.emplace_back(std::async(std::launch::deferred, findT, std::ref(m_processQueue), std::ref(m_searchId), std::ref(m_visitedNodes), m_totalNodes, std::ref(m_checked), std::ref(m_stop)));
        }

        return true;
    }

    bool startSTSearch() {
        if (!m_rootNode)
            return false;

        m_rootNode->markVisited();
        m_visitedNodes.queue(m_rootNode);
        m_processQueue.queue(m_rootNode);
        m_stop = false;

        m_result = ThreadedBFS::findT(m_processQueue, m_searchId, m_visitedNodes, m_totalNodes, m_checked, m_stop);
        return true;
    }


    bool waitForResult() {
        for (auto itr = m_futures.begin(); itr != m_futures.end(); ++itr) {
            auto res = (*itr).get();
            if (res) {
                m_result = res;
                return true;
            }
        }

        m_result = nullptr;
        return false;
    }

    NodeRef getResult() const {
        return m_result;
    }

    void unmarkVisited() {
        while (!m_visitedNodes.empty()) {
            auto node = m_visitedNodes.deque();
            if (node)
                node->markVisited(false);
        }
    }

private:
    NodeRef m_rootNode;
    NodeRef m_result;
    NodeQueue m_processQueue;
    NodeQueue m_visitedNodes;
    NodeRef m_searchResult;
    std::string m_searchId;
    int m_maxThreadCount;
    std::vector<std::future<NodeRef>> m_futures;
    std::atomic_bool m_stop{false};
    size_t m_totalNodes{0};
    std::atomic_int m_checked{0};
};

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

            ThreadedBFS searcher;
            searcher.setSearchId(id);
            searcher.setMaxThreadCount(20);
            searcher.setRootNode(rootNode);
            searcher.setTotalNodeCount(graph->size());

            searcher.startSTSearch();
            //searcher.waitForResult();
            searcher.unmarkVisited();

            return searcher.getResult();;
        }

        NodeRef find2(GraphRef graph, const std::string& id) {
            if (!graph || graph->empty()) {
                Log::error("Graph empty...skipping search");
                return nullptr;
            }

            auto rootNode = graph->getFirst();
            if (rootNode->getId() == id) return rootNode;

            SafeQueue<NodeRef> toProcess;
            SafeQueue<NodeRef> visitedNodes;

            rootNode->markVisited();
            toProcess.queue(rootNode);
            visitedNodes.queue(rootNode);

            NodeRef result;
            std::atomic_bool stop(false);
            std::atomic_int checked{0};
            result = ThreadedBFS::findT(toProcess, id, visitedNodes, graph->size(), checked, stop);
            while(!visitedNodes.empty()) {
                auto node = visitedNodes.deque();
                if (node)
                    node->markVisited(false);
            }

            /*ThreadedBFS searcher;
            searcher.setSearchId(id);
            searcher.setMaxThreadCount(20);
            searcher.setRootNode(rootNode);

            searcher.startMTSearch();

            auto result = searcher.waitForResult();
            searcher.unmarkVisited();*/
            return result;
        }

};
