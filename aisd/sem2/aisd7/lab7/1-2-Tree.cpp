// //1-2-Tree.cpp
// #include "1-2-Tree.h"
// #include <algorithm>
// #include <iostream>

// // Constants for tree display
// const int FIRSTCOL = 80;  // Было 60
// const int MAXCOL = 160;    // Было 120
// const int OFFSET[] = {80, 40, 20, 10, 4, 2};  // Добавили больше уровней
// const int MAXROW = 30;     // Было 24
// const int SHIFT = 3;       // Было 2
// std::string SCREEN[MAXROW];

// // Display methods for Node
// void Node::Out(int row, int col) const {
//     if (row >= MAXROW || col < 0 || col >= MAXCOL) return;
    
//     std::string keyStr = std::to_string(key);
//     int len = std::min(3, (int)keyStr.length());
//     SCREEN[row].replace(col, len, keyStr.substr(0, len));
    
//     if (next && right) {
//         std::string rightStr = std::to_string(right->key);
//         len = std::min(3, (int)rightStr.length());
//         SCREEN[row+1].replace(col+1, len, rightStr.substr(0, len));
//     }
// }

// void Node::Display(int level, int col, int& max_level) const {
//     if (level > max_level) max_level = level;
    
//     int current_offset = (level+1 < (int)(sizeof(OFFSET)/sizeof(OFFSET[0]))) ? 
//                        OFFSET[level+1] : OFFSET[sizeof(OFFSET)/sizeof(OFFSET[0])-1];
    
//     Out(level*2, col);
    
//     if (left) left->Display(level+1, col - current_offset, max_level);
//     if (right) {
//         if (next) {
//             if (right->left) right->left->Display(level+1, col, max_level);
//             if (right->right) right->right->Display(level+1, col + current_offset, max_level);
//         } else {
//             right->Display(level+1, col + current_offset, max_level);
//         }
//     }
// }

// // Tree display method
// void Tree::Display() const {
//     std::fill_n(SCREEN, MAXROW, std::string(MAXCOL, ' '));
//     int max_level = 0;
    
//     if (root) {
//         root->Display(0, FIRSTCOL - SHIFT, max_level);
//     } else {
//         SCREEN[0] = "Empty Tree";
//     }
    
//     // Выводим только заполненные строки
//     for (int i = 0; i <= max_level*2+2 && i < MAXROW; ++i) {
//         // Обрезаем пробелы справа
//         std::string line = SCREEN[i];
//         line.erase(line.find_last_not_of(' ') + 1);
//         std::cout << line << '\n';
//     }
// }

// // Iterator increment operator
// myiter& myiter::operator++() {
//     if (!Ptr) return *this;
    
//     if (Ptr->right) {
//         St.push({Ptr, 1});
//         Ptr = Ptr->right;
//         while (Ptr->left) {
//             St.push({Ptr, 0});
//             Ptr = Ptr->left;
//         }
//     } else {
//         auto pp = std::make_pair(Ptr, 1);
//         while (!St.empty() && pp.second) {
//             pp = St.top();
//             St.pop();
//         }
//         Ptr = pp.second ? nullptr : pp.first;
//     }
//     return *this;
// }

// // Tree iterator methods
// myiter Tree::begin() const {
//     MyStack St;
//     Node* p = root;
//     while (p && p->left) {
//         St.push({p, 0});
//         p = p->left;
//     }
//     return myiter(p, St);
// }

// // Tree insertion method
// std::pair<myiter, bool> Tree::insert(int k, myiter where) {
//     Node *t = root;
//     bool cont = true, up = false;
//     MyStack St;
//     int current_depth = 0;
    
//     if (!where.Ptr) {
//         if (!t) {
//             root = new Node(k);
//             count = 1;
//             tree_height = 1;
//             return {myiter(root), true};
//         }
//         St.push({root, 1});
//         current_depth = 1;
//     } else {
//         t = where.Ptr;
//         St = std::move(where.St);
//         current_depth = St.size();
//     }
    
//     while (cont) {
//         if (k == t->key) {
//             return {myiter(t, std::move(St)), false};
//         }
        
//         if (k < t->key) {
//             if (t->left) {
//                 St.push({t, 2});
//                 t = t->left;
//                 current_depth++;
//             } else {
//                 t->left = new Node(k);
//                 cont = false;
//                 if (current_depth + 1 > tree_height) {
//                     tree_height = current_depth + 1;
//                 }
//             }
//         } else {
//             if (!t->right) {
//                 t->right = new Node(k);
//                 t->next = true;
//                 cont = false;
//                 if (current_depth + 1 > tree_height) {
//                     tree_height = current_depth + 1;
//                 }
//             } else if (t->next) {
//                 if (k < t->right->key) {
//                     if (t->right->left) {
//                         St.push({t, 3});
//                         t = t->right->left;
//                         current_depth++;
//                     } else {
//                         t->right->left = new Node(k);
//                         cont = false;
//                         if (current_depth + 2 > tree_height) {
//                             tree_height = current_depth + 2;
//                         }
//                     }
//                 } else {
//                     if (t->right->right) {
//                         St.push({t, 4});
//                         t = t->right->right;
//                         current_depth++;
//                     } else {
//                         t->right->right = new Node(k);
//                         up = t->right->next = true;
//                         cont = false;
//                         if (current_depth + 2 > tree_height) {
//                             tree_height = current_depth + 2;
//                         }
//                     }
//                 }
//             } else {
//                 if (t->right) {
//                     St.push({t, 3});
//                     t = t->right;
//                     current_depth++;
//                 } else {
//                     t->right = new Node(k);
//                     t->next = true;
//                     cont = false;
//                     if (current_depth + 1 > tree_height) {
//                         tree_height = current_depth + 1;
//                     }
//                 }
//             }
//         }
        
//         while (up) {
//             std::swap(t->key, t->right->key);
//             Node *t1 = t->right;
//             t->next = t1->next = false;
//             t->right = t1->right;
//             t1->right = t1->left;
//             t1->left = t->left;
//             t->left = t1;
//             t1 = t;
            
//             if (St.empty()) {
//                 up = false;
//                 tree_height++;
//             } else {
//                 t = St.top().first;
//                 switch (St.top().second) {
//                     case 2:
//                         std::swap(t->key, t1->key);
//                         t->left = t1->left;
//                         t1->left = t1->right;
//                         t1->right = t->right;
//                         t->right = t1;
//                         up = t1->next = t->next;
//                         break;
//                     case 3:
//                         if (t->next) {
//                             t->right->left = t1->right;
//                             t1->right = t->right;
//                             t->right = t1;
//                             t1->next = true;
//                         } else {
//                             t->next = true;
//                             up = t1->next = false;
//                             St.pop();
//                         }
//                         break;
//                     case 4:
//                         t->right->next = true;
//                         t1->next = false;
//                         break;
//                 }
//                 if (up) St.pop();
//             }
//         }
//     }
    
//     ++count;
//     return {myiter(t, std::move(St)), true};
// }

// // Tree erase method
// bool Tree::erase(int k) {
//     Node *p = nullptr, *q = root;
//     int a = 0;
//     bool cont = (q != nullptr);

//     while (cont && (k != q->key)) {
//         p = q;
//         a = (k > q->key);
//         q = a ? q->right : q->left;
//         cont = (q != nullptr);
//     }

//     if (!cont) return false;

//     Node *r = q->right;
//     if (r && r->left) {
//         p = q;
//         a = 1;
//         do {
//             p = r;
//             r = r->left;
//             a = 0;
//         } while (r->left);
//         q->key = r->key;
//         p->left = r->right;
//         q = r;
//     }
//     else if (r) {
//         (a ? r->right : r->left) = q->left;
//         if (p) (a ? p->right : p->left) = r;
//         else root = r;
//     }
//     else {
//         if (p) {
//             if (a) {
//                 p->right = q->left;
//                 p->next = false;
//             }
//             else {
//                 p->left = q->left;
//             }
//         }
//         else {
//             root = q->left;
//         }
//     }

//     q->left = q->right = nullptr;
//     --count;
//     delete q;
//     return true;
// }

// // Utility function to print a set
// template<typename Iter>
// void printSet(const std::string& name, Iter begin, Iter end) {
//     std::cout << name << " = { ";
//     for (; begin != end; ++begin) {
//         std::cout << *begin << " ";
//     }
//     std::cout << "}\n";
// }

// Tree generate_random_set(int max_size) {
//     Tree result;
//     int size = random_int(1, max_size);
//     while (result.size() < size) {
//         int num = random_int(1, 52);
//         result.insert(num);
//     }
//     return result;
// }

// // Main set operations function
// void execute_set_operations() {
//     try {
//         // Генерация случайных множеств со случайным размером
//         Tree A = generate_random_set(15);
//         Tree B = generate_random_set(10);
//         Tree C = generate_random_set(8);
//         Tree D = generate_random_set(5);
//         Tree E = generate_random_set(5);

//         // Вывод информации о множествах перед их отображением
//         std::cout << "Generated sets:\n";

//         printSet("A", A.begin(), A.end());
//         printSet("B", B.begin(), B.end());
//         printSet("C", C.begin(), C.end());
//         printSet("D", D.begin(), D.end());
//         printSet("E", E.begin(), E.end());
//         std::cout << "----------------------------------\n";

//         // (A minus B)
//         Tree A_minus_B;
//         std::set_difference(A.begin(), A.end(), B.begin(), B.end(),
//                           outinserter(A_minus_B, A_minus_B.begin()));
//         printSet("A minus B", A_minus_B.begin(), A_minus_B.end());
//         std::cout << "----------------------------------\n";

//         // (A minus B) union C
//         Tree A_minus_B_union_C;
//         std::set_union(A_minus_B.begin(), A_minus_B.end(),
//                       C.begin(), C.end(),
//                       outinserter(A_minus_B_union_C, A_minus_B_union_C.begin()));
//         printSet("(A minus B) union C", A_minus_B_union_C.begin(), A_minus_B_union_C.end());
//         std::cout << "----------------------------------\n";

//         // D intersect E
//         Tree D_intersect_E;
//         std::set_intersection(D.begin(), D.end(),
//                              E.begin(), E.end(),
//                              outinserter(D_intersect_E, D_intersect_E.begin()));
//         printSet("D intersect E", D_intersect_E.begin(), D_intersect_E.end());
//         std::cout << "----------------------------------\n";

//         // (A minus B union C) symmetric_diff (D intersect E)
//         Tree result;
//         std::set_symmetric_difference(A_minus_B_union_C.begin(), A_minus_B_union_C.end(),
//                                      D_intersect_E.begin(), D_intersect_E.end(),
//                                      outinserter(result, result.begin()));

//         std::cout << "Final result: (A minus B union C) symmetric_diff (D intersect E)\n";
//         printSet("Result", result.begin(), result.end());
//         std::cout << "Tree visualization:\n";
//         result.Display();
//     } 
//     catch (const std::exception& e) {
//         std::cerr << "Error in set operations: " << e.what() << std::endl;
//         throw;
//     }
// }

// // Main function
// int main() {
//     try {
//         std::cout << "Starting set operations...\n";
//         execute_set_operations();
//         std::cout << "Operations completed successfully.\n";
//         std::cin.get();
//         return 0;
//     } 
//     catch (const std::exception& e) {
//         std::cerr << "Fatal error: " << e.what() << std::endl;
//         return 1;
//     }
// }

// 1-2-Tree.cpp
#include "1-2-Tree.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

// Constants for tree display
const int FIRSTCOL = 80;
const int MAXCOL = 160;
const int OFFSET[] = {40, 20, 10, 5, 2, 1};
const int MAXROW = 30;
const int SHIFT = 3;
std::string SCREEN[MAXROW];

// Display methods for Node with bounds checking
void Node::Out(int row, int col) const {
    if (row >= MAXROW || col < 0 || col >= MAXCOL) return;
    
    std::string keyStr = std::to_string(key);
    if (count > 1) {
        keyStr += "(" + std::to_string(count) + ")";
    }
    
    // Ensure we don't write past screen boundaries
    int len = std::min(static_cast<int>(keyStr.length()), MAXCOL - col);
    if (len > 0) {
        SCREEN[row].replace(col, len, keyStr.substr(0, len));
    }
}

// Simplified tree display method
void Node::Display(int level, int col, int& max_level) const {
    if (level > max_level) max_level = level;
    
    // Calculate offset based on level with bounds checking
    int current_offset = (level < 6) ? OFFSET[level] : OFFSET[5];
    
    // Display current node
    Out(level*2, col);
    
    // Recursively display left and right subtrees
    if (left) left->Display(level+1, col - current_offset, max_level);
    if (right) right->Display(level+1, col + current_offset, max_level);
}

// Tree display method with proper initialization
void Tree::Display() const {
    // Clear the screen buffer
    std::fill_n(SCREEN, MAXROW, std::string(MAXCOL, ' '));
    int max_level = 0;
    
    if (root) {
        root->Display(0, FIRSTCOL, max_level);
    } else {
        SCREEN[0].replace(FIRSTCOL, 9, "Empty Tree");
    }
    
    // Display only the relevant portion of the screen
    for (int i = 0; i <= max_level*2+2 && i < MAXROW; ++i) {
        std::string line = SCREEN[i];
        // Trim trailing spaces
        line.erase(line.find_last_not_of(' ') + 1);
        std::cout << line << '\n';
    }
}

// Iterator increment operator
myiter& myiter::operator++() {
    if (!Ptr) return *this;
    
    if (current_count < Ptr->count) {
        current_count++;
        return *this;
    }
    
    if (Ptr->right) {
        St.push({Ptr, 1});
        Ptr = Ptr->right;
        while (Ptr->left) {
            St.push({Ptr, 0});
            Ptr = Ptr->left;
        }
    } else {
        auto pp = std::make_pair(Ptr, 1);
        while (!St.empty() && pp.second) {
            pp = St.top();
            St.pop();
        }
        Ptr = pp.second ? nullptr : pp.first;
    }
    
    current_count = 1;
    return *this;
}

// Tree iterator methods
myiter Tree::begin() const {
    MyStack St;
    Node* p = root;
    while (p && p->left) {
        St.push({p, 0});
        p = p->left;
    }
    return myiter(p, St);
}

// Tree insertion method
std::pair<myiter, bool> Tree::insert(int k, myiter where) {
    Node *t = root;
    bool cont = true, up = false;
    MyStack St;
    int current_depth = 0;
    
    if (!where.Ptr) {
        if (!t) {
            root = new Node(k);
            count = 1;
            tree_height = 1;
            return {myiter(root), true};
        }
        St.push({root, 1});
        current_depth = 1;
    } else {
        t = where.Ptr;
        St = std::move(where.St);
        current_depth = St.size();
    }
    
    while (cont) {
        if (k == t->key) {
            if (is_set) {
                return {myiter(t, std::move(St)), false};
            } else {
                t->count++;
                count++;
                return {myiter(t, std::move(St)), false};
            }
        }
        
        if (k < t->key) {
            if (t->left) {
                St.push({t, 2});
                t = t->left;
                current_depth++;
            } else {
                t->left = new Node(k);
                cont = false;
                if (current_depth + 1 > tree_height) {
                    tree_height = current_depth + 1;
                }
            }
        } else {
            if (!t->right) {
                t->right = new Node(k);
                t->next = true;
                cont = false;
                if (current_depth + 1 > tree_height) {
                    tree_height = current_depth + 1;
                }
            } else if (t->next) {
                if (k < t->right->key) {
                    if (t->right->left) {
                        St.push({t, 3});
                        t = t->right->left;
                        current_depth++;
                    } else {
                        t->right->left = new Node(k);
                        cont = false;
                        if (current_depth + 2 > tree_height) {
                            tree_height = current_depth + 2;
                        }
                    }
                } else {
                    if (t->right->right) {
                        St.push({t, 4});
                        t = t->right->right;
                        current_depth++;
                    } else {
                        t->right->right = new Node(k);
                        up = t->right->next = true;
                        cont = false;
                        if (current_depth + 2 > tree_height) {
                            tree_height = current_depth + 2;
                        }
                    }
                }
            } else {
                if (t->right) {
                    St.push({t, 3});
                    t = t->right;
                    current_depth++;
                } else {
                    t->right = new Node(k);
                    t->next = true;
                    cont = false;
                    if (current_depth + 1 > tree_height) {
                        tree_height = current_depth + 1;
                    }
                }
            }
        }
        
        while (up) {
            std::swap(t->key, t->right->key);
            std::swap(t->count, t->right->count);
            Node *t1 = t->right;
            t->next = t1->next = false;
            t->right = t1->right;
            t1->right = t1->left;
            t1->left = t->left;
            t->left = t1;
            t1 = t;
            
            if (St.empty()) {
                up = false;
                tree_height++;
            } else {
                t = St.top().first;
                switch (St.top().second) {
                    case 2:
                        std::swap(t->key, t1->key);
                        std::swap(t->count, t1->count);
                        t->left = t1->left;
                        t1->left = t1->right;
                        t1->right = t->right;
                        t->right = t1;
                        up = t1->next = t->next;
                        break;
                    case 3:
                        if (t->next) {
                            t->right->left = t1->right;
                            t1->right = t->right;
                            t->right = t1;
                            t1->next = true;
                        } else {
                            t->next = true;
                            up = t1->next = false;
                            St.pop();
                        }
                        break;
                    case 4:
                        t->right->next = true;
                        t1->next = false;
                        break;
                }
                if (up) St.pop();
            }
        }
    }
    
    ++count;
    return {myiter(t, std::move(St)), true};
}

// Tree erase method (removes one occurrence)
bool Tree::erase(int k) {
    Node *p = nullptr, *q = root;
    int a = 0;
    bool cont = (q != nullptr);

    while (cont && (k != q->key)) {
        p = q;
        a = (k > q->key);
        q = a ? q->right : q->left;
        cont = (q != nullptr);
    }

    if (!cont) return false;

    if (!is_set || q->count > 1) {
        q->count--;
        count--;
        if (q->count == 0) {
            // Should never happen for sets
            Node *r = q->right;
            if (r && r->left) {
                p = q;
                a = 1;
                do {
                    p = r;
                    r = r->left;
                    a = 0;
                } while (r->left);
                q->key = r->key;
                q->count = r->count;
                p->left = r->right;
                q = r;
            }
            else if (r) {
                (a ? r->right : r->left) = q->left;
                if (p) (a ? p->right : p->left) = r;
                else root = r;
            }
            else {
                if (p) {
                    if (a) {
                        p->right = q->left;
                        p->next = false;
                    }
                    else {
                        p->left = q->left;
                    }
                }
                else {
                    root = q->left;
                }
            }

            q->left = q->right = nullptr;
            delete q;
        }
        return true;
    }

    Node *r = q->right;
    if (r && r->left) {
        p = q;
        a = 1;
        do {
            p = r;
            r = r->left;
            a = 0;
        } while (r->left);
        q->key = r->key;
        q->count = r->count;
        p->left = r->right;
        q = r;
    }
    else if (r) {
        (a ? r->right : r->left) = q->left;
        if (p) (a ? p->right : p->left) = r;
        else root = r;
    }
    else {
        if (p) {
            if (a) {
                p->right = q->left;
                p->next = false;
            }
            else {
                p->left = q->left;
            }
        }
        else {
            root = q->left;
        }
    }

    q->left = q->right = nullptr;
    --count;
    delete q;
    return true;
}

// New method to remove all occurrences
bool Tree::erase_all(int k) {
    Node *p = nullptr, *q = root;
    int a = 0;
    bool cont = (q != nullptr);

    while (cont && (k != q->key)) {
        p = q;
        a = (k > q->key);
        q = a ? q->right : q->left;
        cont = (q != nullptr);
    }

    if (!cont) return false;

    int removed_count = q->count;
    Node *r = q->right;
    if (r && r->left) {
        p = q;
        a = 1;
        do {
            p = r;
            r = r->left;
            a = 0;
        } while (r->left);
        q->key = r->key;
        q->count = r->count;
        p->left = r->right;
        q = r;
    }
    else if (r) {
        (a ? r->right : r->left) = q->left;
        if (p) (a ? p->right : p->left) = r;
        else root = r;
    }
    else {
        if (p) {
            if (a) {
                p->right = q->left;
                p->next = false;
            }
            else {
                p->left = q->left;
            }
        }
        else {
            root = q->left;
        }
    }

    q->left = q->right = nullptr;
    count -= removed_count;
    delete q;
    return true;
}

// Utility function to print a set
template<typename Iter>
void printSet(const std::string& name, Iter begin, Iter end) {
    std::cout << name << " = { ";
    for (; begin != end; ++begin) {
        std::cout << *begin << " ";
    }
    std::cout << "}\n";
}

Tree generate_random_set(int max_size) {
    Tree result(true); // Set mode
    int size = random_int(1, max_size);
    while (result.size() < size) {
        int num = random_int(1, 52);
        result.insert(num);
    }
    return result;
}

Tree generate_random_sequence(int max_size) {
    Tree result(false); // Sequence mode
    int size = random_int(1, max_size);
    for (int i = 0; i < size; ++i) {
        int num = random_int(1, 20); // Smaller range to increase duplicates
        result.insert(num);
    }
    return result;
}

// Fixed sequence operations implementation
void execute_sequence_operations() {
    try {
        int choice = 0;
        std::cout << "\nSequence Operations:\n";
        std::cout << "1. Merge two sequences\n";
        std::cout << "2. Concatenate two sequences\n";
        std::cout << "3. Erase range from sequence\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        // Generate only one sequence for erase operation
        Tree seq1 = generate_random_sequence(10);
        
        if (choice == 3) {
            // Handle erase range operation
            std::cout << "\nGenerated sequence:\n";
            printSet("Sequence", seq1.begin(), seq1.end());
            
            int start, end;
            std::cout << "Enter start and end positions to erase (1-based): ";
            std::cin >> start >> end;
            
            if (start < 1 || end > seq1.size() || start > end) {
                std::cout << "Invalid range!\n";
                return;
            }
            
            // Convert to vector for easier range deletion
            std::vector<int> elements(seq1.begin(), seq1.end());
            elements.erase(elements.begin() + start - 1, elements.begin() + end);
            
            // Rebuild the tree
            seq1.clear();
            for (int val : elements) {
                seq1.insert(val);
            }
            
            printSet("Sequence after erase", seq1.begin(), seq1.end());
            std::cout << "Tree visualization:\n";
            seq1.Display();
        } 
        else {
            // Handle merge and concat operations
            Tree seq2 = generate_random_sequence(8);
            std::cout << "\nGenerated sequences:\n";
            printSet("Sequence 1", seq1.begin(), seq1.end());
            printSet("Sequence 2", seq2.begin(), seq2.end());
            
            Tree result(false); // Sequence mode
            
            if (choice == 1) {
                // Merge operation
                std::vector<int> temp1(seq1.begin(), seq1.end());
                std::vector<int> temp2(seq2.begin(), seq2.end());
                std::sort(temp1.begin(), temp1.end());
                std::sort(temp2.begin(), temp2.end());
                std::merge(temp1.begin(), temp1.end(),
                         temp2.begin(), temp2.end(),
                         outinserter(result, result.begin()));
                printSet("Merged sequence", result.begin(), result.end());
            } 
            else if (choice == 2) {
                // for (auto it = seq1.begin(); it != seq1.end(); ++it) {
                //     result.insert(*it);
                // }
                // for (auto it = seq2.begin(); it != seq2.end(); ++it) {
                //     result.insert(*it);
                // }
                std::vector<int> result;
                std::copy(seq1.begin(), seq1.end(), std::back_inserter(result));
                std::copy(seq2.begin(), seq2.end(), std::back_inserter(result));
                printSet("Concatenated sequence", result.begin(), result.end());
            } 
            else {
                std::cout << "Invalid choice!\n";
                return;
            }
            
            std::cout << "Tree visualization:\n";
            result.Display();
        }
    } 
    catch (const std::exception& e) {
        std::cerr << "Error in sequence operations: " << e.what() << std::endl;
    }
}

// Main set operations function
void execute_set_operations() {
    try {
        Tree A = generate_random_set(15);
        Tree B = generate_random_set(10);
        Tree C = generate_random_set(8);
        Tree D = generate_random_set(5);
        Tree E = generate_random_set(5);

        std::cout << "Generated sets:\n";
        printSet("A", A.begin(), A.end());
        printSet("B", B.begin(), B.end());
        printSet("C", C.begin(), C.end());
        printSet("D", D.begin(), D.end());
        printSet("E", E.begin(), E.end());
        std::cout << "----------------------------------\n";

        Tree A_minus_B(true);
        std::set_difference(A.begin(), A.end(), B.begin(), B.end(),
                          outinserter(A_minus_B, A_minus_B.begin()));
        printSet("A minus B", A_minus_B.begin(), A_minus_B.end());
        std::cout << "----------------------------------\n";

        Tree A_minus_B_union_C(true);
        std::set_union(A_minus_B.begin(), A_minus_B.end(),
                      C.begin(), C.end(),
                      outinserter(A_minus_B_union_C, A_minus_B_union_C.begin()));
        printSet("(A minus B) union C", A_minus_B_union_C.begin(), A_minus_B_union_C.end());
        std::cout << "----------------------------------\n";

        Tree D_intersect_E(true);
        std::set_intersection(D.begin(), D.end(),
                             E.begin(), E.end(),
                             outinserter(D_intersect_E, D_intersect_E.begin()));
        printSet("D intersect E", D_intersect_E.begin(), D_intersect_E.end());
        std::cout << "----------------------------------\n";

        Tree result(true);
        std::set_symmetric_difference(A_minus_B_union_C.begin(), A_minus_B_union_C.end(),
                                     D_intersect_E.begin(), D_intersect_E.end(),
                                     outinserter(result, result.begin()));

        std::cout << "Final result: (A minus B union C) symmetric_diff (D intersect E)\n";
        printSet("Result", result.begin(), result.end());
        std::cout << "Tree visualization:\n";
        result.Display();
    } 
    catch (const std::exception& e) {
        std::cerr << "Error in set operations: " << e.what() << std::endl;
        throw;
    }
}

// Main function
int main() {
    try {
        int choice = 0;
        while (true) {
            std::cout << "\nMenu:\n";
            std::cout << "1. Set operations\n";
            std::cout << "2. Sequence operations\n";
            std::cout << "3. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            switch (choice) {
                case 1:
                    execute_set_operations();
                    break;
                case 2:
                    execute_sequence_operations();
                    break;
                case 3:
                    return 0;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    } 
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
