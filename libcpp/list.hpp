//
// Created by Harold on 2020/4/21.
//

#ifndef OS_CPP_LIST_HPP
#define OS_CPP_LIST_HPP

#include <sys/types.hpp>
#include <libcpp/assert.hpp>

    class ListNode {
        ListNode *next{};
        ListNode *prev{};

        template<typename Elem, ListNode Elem::*list_node_ptr>
        friend
        class List;

        void *list{};
    };

    template<typename Elem, ListNode Elem::*list_node_ptr>
    class List {
        size_t size_ = 0;
        ListNode *head_ = nullptr;
        ListNode *tail_ = nullptr;

        constexpr uint32_t offset() const {
            return reinterpret_cast<uint32_t>(
                    &(reinterpret_cast<Elem *>(0)->*list_node_ptr));
        }

        Elem &object(const ListNode *list_node) const {
            auto &o = *reinterpret_cast<Elem *>(
                    reinterpret_cast<uint32_t>(list_node) - offset());
            return o;
        }

    public:
        List() = default;

        List(const List &rhs) = delete;

        List &operator=(const List &rhs) = delete;

        class Iterator {
            friend class List;

            const List &list;
            ListNode *cur = nullptr;

            explicit Iterator(const List &list, ListNode *begin)
                    : list(list), cur(begin) {}

        public:
            Iterator() = default;


            bool operator==(const Iterator &rhs) {
                return &list == &rhs.list && cur == rhs.cur;
            }

            bool operator!=(const Iterator &rhs) {
                return &list == &rhs.list && cur != rhs.cur;
            }

            const Iterator &operator++() {
                cur = cur->next;
                return *this;
            }

            Elem &operator*() { return list.object(cur); }
        };

        Iterator begin() { return Iterator(*this, head_); }

        Iterator end() { return Iterator(*this, nullptr); }

        void append(Elem &);

        Elem &pop();

        void left_append(Elem &);

        Elem &left_pop();

        void remove(Elem &);

        Elem &head() {
            assert(head_ != nullptr);
            return object(head_);
        }

        Elem &tail() {
            assert(tail_ != nullptr);
            return object(tail_);
        }

        size_t size() { return size_; }

        bool empty() { return !size_; }

    };

    template<typename Elem, ListNode Elem::*list_node_ptr>
    void List<Elem, list_node_ptr>::append(Elem &elem) {
        ListNode *node = &(elem.*list_node_ptr);
        assert(node->prev == nullptr && node->next == nullptr);
        if (tail_) {
            tail_->next = node;
            node->prev = tail_;
            node->next = nullptr;
            tail_ = node;
        } else {
            assert(!head_);
            head_ = tail_ = node;
            node->prev = node->next = nullptr;
        }
        node->list = this;
        ++size_;
    }

    template<typename Elem, ListNode Elem::*list_node_ptr>
    void List<Elem, list_node_ptr>::left_append(Elem &elem) {
        ListNode *node = &(elem.*list_node_ptr);
        if (head_) {
            node->next = head_;
            node->prev = nullptr;
            head_ = node;
        } else {
            assert(!tail_);
            head_ = tail_ = node;
            node->next = nullptr;
            node->prev = nullptr;
        }
        node->list = this;
        ++size_;
    }

    template<typename Elem, ListNode Elem::*list_node_ptr>
    Elem &List<Elem, list_node_ptr>::pop() {
        assert(tail_);
        auto *t = tail_;
        tail_ = tail_->prev;
        if (tail_) {
            tail_->next = nullptr;
        } else {
            head_ = nullptr;
        }
        --size_;
        t->next = t->prev = nullptr;
        t->list = nullptr;
        return object(t);
    }

    template<typename Elem, ListNode Elem::*list_node_ptr>
    Elem &List<Elem, list_node_ptr>::left_pop() {
        assert(head_ != nullptr);
        auto *t = head_;
        head_ = head_->next;
        if (head_) {
            head_->prev = nullptr;
        } else {
            tail_ = nullptr;
        }
        --size_;
        t->prev = t->next = nullptr;
        t->list = nullptr;
        return object(t);
    }

    template<typename Elem, ListNode Elem::*list_node_ptr>
    void List<Elem, list_node_ptr>::remove(Elem &elem) {
        ListNode *node = &(elem.*list_node_ptr);
        if (head_ == node) {
            left_pop();
        } else if (tail_ == node) {
            pop();
        } else if ((elem.*list_node_ptr).list == this) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->prev = node->next = nullptr;
            --size_;
        } else {
            PANIC("Element not in the list!");
        }
    }


#endif  // OS_CPP_LIST_HPP
