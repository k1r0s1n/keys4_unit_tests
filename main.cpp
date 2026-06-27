

#include <vector>
#include <stdexcept> 
#include <algorithm> 
#include <deque>     


template<typename T>
class Queue {
private:
    std::deque<T> data; 

public:
    void push(const T& item) {
        data.push_back(item);
    }

    T pop() {
        if (data.empty()) {
            throw std::runtime_error("Queue is empty");
        }
        T front = data.front();
        data.pop_front(); 
        return front;
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};


template<typename T>
class Heap {
private:
    std::vector<T> data;

public:
    void push(const T& item) {
        data.push_back(item);
        std::push_heap(data.begin(), data.end()); 
    }

    T pop() {
        if (data.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        std::pop_heap(data.begin(), data.end()); 
        T top_item = data.back();
        data.pop_back(); 
        return top_item;
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }

    T top() const {
        if (data.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return data[0]; 
    }
};


template<typename T>
class BinaryTree {
public:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };

private:
    Node* root;
    void destroy_tree(Node* node) {
        if (node) {
            destroy_tree(node->left);
            destroy_tree(node->right);
            delete node;
        }
    }

    Node* insert_recursive(Node* node, const T& item) {
        if (!node) {
            return new Node(item);
        }
        if (item < node->data) {
            node->left = insert_recursive(node->left, item);
        } else if (item > node->data) {
             node->right = insert_recursive(node->right, item);
        } 
        return node;
    }

    Node* find_min_node(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    
    
    bool remove_recursive(Node*& node, const T& item) {
         if (!node) {
            return false; 
         }

         if (item < node->data) {
             return remove_recursive(node->left, item);
         } else if (item > node->data) {
              return remove_recursive(node->right, item);
         } else { 
             Node* node_to_delete = node; 
             
             if (!node->left && !node->right) {
                 node = nullptr; 
             }
             
             else if (!node->left) {
                  node = node->right; 
             }
             else if (!node->right) {
                  node = node->left;  
             }
             
             else {
                 Node* successor = find_min_node(node->right); 
                 node->data = successor->data; 
                 
                 
                 
                 bool successor_found_and_removed = remove_recursive(node->right, successor->data);
                 
                 
                 
                 
                 
                 delete node_to_delete; 
                 return true; 
             }
             delete node_to_delete; 
             return true; 
         }
         
         
    }


     bool search_recursive(const Node* node, const T& item) const {
          if (!node) {
              return false;
          }
          if (item == node->data) {
               return true;
          }
          if (item < node->data) {
              return search_recursive(node->left, item);
          } else {
              return search_recursive(node->right, item);
          }
     }


public:
    BinaryTree() : root(nullptr) {}

    void push(const T& item) {
         root = insert_recursive(root, item);
    }

    
    bool pop(const T& item) {
         return remove_recursive(root, item); 
    }


    bool search(const T& item) const {
         return search_recursive(root, item);
    }

    ~BinaryTree() {
        destroy_tree(root);
    }
};



#include <gtest/gtest.h>



template<typename T>
class QueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        queue = new Queue<T>;
    }

    void TearDown() override {
        delete queue;
    }
    Queue<T>* queue;
};

using TestTypes = ::testing::Types<int, std::string>;
TYPED_TEST_SUITE(QueueTest, TestTypes);

TYPED_TEST(QueueTest, IsEmptyInitially) {
    EXPECT_TRUE(this->queue->empty());
    EXPECT_EQ(this->queue->size(), 0);
}

TYPED_TEST(QueueTest, PushIncreasesSizeAndIsNotEmpty) {
    this->queue->push(TypeParam{});
    EXPECT_FALSE(this->queue->empty());
    EXPECT_EQ(this->queue->size(), 1);
}

TYPED_TEST(QueueTest, PushThenPopReturnsSameElement) {
    TypeParam element = TypeParam{}; 
    this->queue->push(element);
    TypeParam popped_element = this->queue->pop();
    EXPECT_EQ(popped_element, element);
    EXPECT_TRUE(this->queue->empty());
}

TYPED_TEST(QueueTest, MultiplePushesPopsInFIFOOrder) {
    TypeParam elem1{}, elem2{}, elem3{}; 
    if constexpr (std::is_arithmetic_v<TypeParam>) {
         elem1 = static_cast<TypeParam>(1);
         elem2 = static_cast<TypeParam>(2);
         elem3 = static_cast<TypeParam>(3);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
         elem1 = "first";
         elem2 = "second";
         elem3 = "third";
    }

    this->queue->push(elem1);
    this->queue->push(elem2);
    this->queue->push(elem3);

    EXPECT_EQ(this->queue->size(), 3);
    EXPECT_EQ(this->queue->pop(), elem1); 
    EXPECT_EQ(this->queue->pop(), elem2);
    EXPECT_EQ(this->queue->pop(), elem3);
    EXPECT_TRUE(this->queue->empty());
}

TYPED_TEST(QueueTest, SizeUpdatesCorrectly) {
    EXPECT_EQ(this->queue->size(), 0);
    this->queue->push(TypeParam{});
    EXPECT_EQ(this->queue->size(), 1);
    this->queue->push(TypeParam{});
    EXPECT_EQ(this->queue->size(), 2);
    this->queue->pop();
    EXPECT_EQ(this->queue->size(), 1);
    this->queue->pop();
    EXPECT_EQ(this->queue->size(), 0);
}




template<typename T>
class HeapTest : public ::testing::Test {
protected:
    void SetUp() override {
        heap = new Heap<T>;
    }

    void TearDown() override {
        delete heap;
    }
    Heap<T>* heap;
};

TYPED_TEST_SUITE(HeapTest, TestTypes);

TYPED_TEST(HeapTest, IsEmptyInitially) {
    EXPECT_TRUE(this->heap->empty());
    EXPECT_EQ(this->heap->size(), 0);
}

TYPED_TEST(HeapTest, PushIncreasesSizeAndIsNotEmpty) {
    this->heap->push(TypeParam{});
    EXPECT_FALSE(this->heap->empty());
    EXPECT_EQ(this->heap->size(), 1);
    
    
    
    EXPECT_EQ(this->heap->top(), TypeParam{});
}

TYPED_TEST(HeapTest, PopRemovesTopElement) {
    TypeParam elem1 = TypeParam{}, elem2 = TypeParam{};
    if constexpr (std::is_arithmetic_v<TypeParam>) {
         elem1 = static_cast<TypeParam>(10);
         elem2 = static_cast<TypeParam>(20);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
         elem1 = "a"; 
         elem2 = "b";
    }

    this->heap->push(elem1);
    this->heap->push(elem2); 

    EXPECT_EQ(this->heap->size(), 2);
    TypeParam top_elem = this->heap->top();
    
    if constexpr (std::is_arithmetic_v<TypeParam>) {
        EXPECT_GE(top_elem, elem1);
        EXPECT_GE(top_elem, elem2);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
        EXPECT_GE(top_elem, elem1);
        EXPECT_GE(top_elem, elem2);
    }
    TypeParam popped_elem = this->heap->pop();
    EXPECT_EQ(popped_elem, top_elem); 
    EXPECT_EQ(this->heap->size(), 1);
}

TYPED_TEST(HeapTest, SizeUpdatesCorrectly) {
    EXPECT_EQ(this->heap->size(), 0);
    this->heap->push(TypeParam{});
    EXPECT_EQ(this->heap->size(), 1);
    this->heap->push(TypeParam{});
    EXPECT_EQ(this->heap->size(), 2);
    this->heap->pop();
    EXPECT_EQ(this->heap->size(), 1);
    this->heap->pop();
    EXPECT_EQ(this->heap->size(), 0);
}



template<typename T>
class BinaryTreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        tree = new BinaryTree<T>;
    }

    void TearDown() override {
        delete tree;
    }
    BinaryTree<T>* tree;
};

TYPED_TEST_SUITE(BinaryTreeTest, TestTypes);

TYPED_TEST(BinaryTreeTest, SearchEmptyTreeReturnsFalse) {
    TypeParam element = TypeParam{}; 
    if constexpr (std::is_arithmetic_v<TypeParam>) {
         element = static_cast<TypeParam>(42);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
         element = "not_found";
    }
    EXPECT_FALSE(this->tree->search(element));
}

TYPED_TEST(BinaryTreeTest, PushThenSearchReturnsTrue) {
    TypeParam element = TypeParam{}; 
    if constexpr (std::is_arithmetic_v<TypeParam>) {
         element = static_cast<TypeParam>(100);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
         element = "found";
    }
    this->tree->push(element);
    EXPECT_TRUE(this->tree->search(element));
}

TYPED_TEST(BinaryTreeTest, SearchNonExistentElementReturnsFalse) {
    TypeParam elem1 = TypeParam{}, elem2 = TypeParam{}; 
    if constexpr (std::is_arithmetic_v<TypeParam>) {
         elem1 = static_cast<TypeParam>(5);
         elem2 = static_cast<TypeParam>(10);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
         elem1 = "exists";
         elem2 = "does_not_exist";
    }
    this->tree->push(elem1);
    EXPECT_FALSE(this->tree->search(elem2)); 
}

TYPED_TEST(BinaryTreeTest, PushThenPopReturnsTrueAndElementNotFound) {
    TypeParam element = TypeParam{}; 
    if constexpr (std::is_arithmetic_v<TypeParam>) {
         element = static_cast<TypeParam>(99);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
         element = "to_be_removed";
    }
    this->tree->push(element);
    EXPECT_TRUE(this->tree->search(element));
    bool result = this->tree->pop(element);
    EXPECT_TRUE(result); 
    EXPECT_FALSE(this->tree->search(element)); 
}

TYPED_TEST(BinaryTreeTest, PopNonExistentElementReturnsFalse) {
    TypeParam elem1 = TypeParam{}, elem2 = TypeParam{}; 
    if constexpr (std::is_arithmetic_v<TypeParam>) {
         elem1 = static_cast<TypeParam>(8);
         elem2 = static_cast<TypeParam>(15);
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
         elem1 = "present";
         elem2 = "absent";
    }
    this->tree->push(elem1);
    bool result = this->tree->pop(elem2); 
    EXPECT_FALSE(result); 
    EXPECT_TRUE(this->tree->search(elem1)); 
}