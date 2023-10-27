#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

class Node
{
    int key, balance;
    Node *left, *right;
    public:
    Node(int k=0, Node*l=NULL,Node*r=NULL, int b=0){key=k; balance=b; left=l; right= r;}
    friend void rec_del(Node*);
    friend class FTree;
    friend class STree;
};

void rec_del(Node* q)
{
    if(q==NULL) return;
    if(q->left) rec_del(q->left);
    if(q->right) rec_del(q->right);
    delete q;
}


class FTree
{
    protected:
    Node *root;
    public:

    FTree(){root = NULL;}
    FTree(int);
    FTree(const vector<int> &arr);
    FTree(const FTree&);
    ~FTree();

    Node* get_root(){return root;}

    Node* CopyTree(Node*);
    FTree operator =(const FTree&);

    Node* Find_k(int);

    void Add(int);
    void Del(int);

    int min();
    int max();

    int KRL(int *a);
    void inarray(Node* q, int* a,int& k);
    list<Node*> trav_level();
    
    int GetHeight();
    void GetHeightRec(Node*, int, int&);
    vector<int> FindLevels(int);
    void FindLevelsRec(Node*, int, int&,vector<int>&);

    void print(ostream& os, int, Node*) const;
    friend ostream & operator << (ostream&, const FTree&);
    Node* FindParent(Node*);
};

class STree: public FTree
{
    public:
    STree(int, bool b=0);
    Node* rotate_right(Node*, Node*);
    Node* rotate_left(Node*, Node*);
    Node* AddWithBalance(Node*, int, bool&);
};


FTree::FTree(int n){
    if(n){
        Node* q = new Node(rand() % 100);
        int k;
        for (int i = 1; i < n; i++) {
            k = rand() % 100;
            Node* p = q;
            Node* g = p;
            while (p != NULL) {
                if (k < p->key) {
                    g = p;
                    p = p->left;
                }
                else {
                    g = p;
                    p = p->right;
                }
            }
            p = new Node(k);
            if (k < g->key) {
                g->left = p;
            }
            else {
                g->right = p;
            }
        }
        root = q;
    } else root=NULL;
}

FTree::FTree(const vector<int> &arr){
    if(arr.size()>0){
        Node* q = new Node(arr[0]);
        int k; int sz=arr.size();
        vector<int>::iterator i;
        for (int i = 1; i < sz; i++) {
            k = arr[i];
            Node* p = q;
            Node* g = p;
            while (p != NULL) {
                if (k < p->key) {
                    g = p;
                    p = p->left;
                }
                else {
                    g = p;
                    p = p->right;
                }
            }
            p = new Node(k);
            if (k < g->key) {
                g->left = p;
            }
            else {
                g->right = p;
            }
        }
        root = q;
    } else root=NULL;
}

FTree::FTree(const FTree &FT){
    if(FT.root==NULL) root=NULL;
    else{
        root=new Node(FT.root->key);
        root->left = CopyTree(FT.root->left);
		root->right = CopyTree(FT.root->right);
    }
}

FTree::~FTree(){
    rec_del(root); root=NULL;
}

Node* FTree::CopyTree(Node *q){
    if(q==NULL) return NULL;
    Node *p=new Node(q->key);
    p->left=CopyTree(q->left);
    p->right=CopyTree(q->right);
    return p;
}

FTree FTree::operator=(const FTree&FT){
    if (this == &FT) return *this;
    this->~FTree();
    if (FT.root == NULL) root = NULL;
    else {
        root = new Node(FT.root->key);
        root->left = CopyTree(FT.root->left);
        root->right = CopyTree(FT.root->right);
    }
    return *this;
}

Node* FTree::Find_k(int k){
    Node* tmp = root;
    while (tmp!=NULL&&tmp->key!=k) {
        if (k < tmp->key) tmp = tmp->left;
        if (k > tmp->key) tmp = tmp->right;
    }
    return tmp;
}

void FTree::Add(int k){
    Node* a = new Node(k);
    if (root == NULL) {
        root = a;
        return;
    }
    Node* q=root;
    while(1){
        if (a->key < q->key) {
            if (q->left == NULL) break;
            q = q->left;
        } else {
            if (q->right == NULL) break;
            q = q->right;
        }
    }
    if(a->key<q->key) q->left=a;
    else q->right=a;
}

void FTree::Del(int k){
    Node* p;
    Node* q=root;
    while(q!=NULL&&q->key!=k){
        p=q;
        if(k<q->key) q=q->left;
        if(k>q->key) q=q->right;
    }
    if(q==NULL) return;
    if(q->left==NULL&&q->right==NULL){
        if(p!=NULL){
            if(p->left==q) p->left=NULL;
            else p->right=NULL;
        }   
        else root=NULL;
        delete q;
        return;
    }
    if(q->left!=NULL&&q->right!=NULL){
        Node* replace_p=q;
        Node *replace=q->left;
        while(replace->right){
            replace_p=replace;
            replace=replace->right;
        }
        q->key=replace->key;
        if(replace_p==q) q->left=replace->left;
        else replace_p->right=replace->left;
        delete replace;
        return;
    }
    Node* replace;
    if (q->left!=NULL) replace = q->left;
    else replace=q->right;
    if(k<p->key) p->left=replace;
    else p->right=replace;
    delete q;
}


int FTree::max(){
    if (root == NULL) {
        cout<<"Дерево пустое";
        return -1;
    }
    Node* max = root;
    while (max->right != NULL) max = max->right;
    return max->key;
}

int FTree::min(){
    if (root == NULL) {
        cout<<"Дерево пустое";
        return -1;
    }
    Node* min = root;
    while (min->left != NULL) min = min->left;
    return min->key;
}

vector<int> FTree::FindLevels(int key) {
    vector<int> arr;
    if (root) FindLevelsRec(root,0,key,arr);
    sort(arr.begin(), arr.end());
    vector<int>::iterator it;
    it=unique(arr.begin(),arr.end());
    arr.resize(distance(arr.begin(), it));
    return arr;
}

void FTree::FindLevelsRec(Node* a, int k, int& key,vector<int>&arr) {
    if(a->key==key) arr.push_back(k);
    if (a->left) FindLevelsRec(a->left, k+1, key, arr);
    if (a->right) FindLevelsRec(a->right, k+1, key, arr);
}


int FTree::GetHeight() {
    int max = 0;
    GetHeightRec(root, 0, max);
    return max;
}

void FTree::GetHeightRec(Node* a, int k, int& max) {
    if (max < k) max = k;
    if (a->left) GetHeightRec(a->left, k+1, max);
    if (a->right) GetHeightRec(a->right, k+1, max);
}


int FTree::KRL(int *a){
    int k=0;
    inarray(root,a,k);
    return k;
}

void FTree::inarray(Node*q, int*a, int&k){
    if(q==NULL) return;
    a[k++]=q->key;
    inarray(q->right,a,k);
    inarray(q->left,a,k);
}

list<Node*> FTree::trav_level(){
    list<Node*> t;
    t.push_back(root);
    list<Node*>::iterator h=t.begin();
    while(h!=t.end()){
        if((*h)->left) t.push_back((*h)->left);
        if((*h)->right) t.push_back((*h)->right);
        cout<<(*h)->key<<" ";
        h++;
    }
    return t;
}

void FTree::print(ostream& out, int spaces, Node* node) const{
    if (node == NULL) {
        return;
    }
    print(out, spaces+4, node->right);

    for (int i = 0; i < spaces; i++) {
        out << " ";
    }
    out << node->key << endl;

    print(out, spaces+4, node->left);
}

ostream& operator<<(ostream& out, const FTree& FT) {
    FT.print(out, 0, FT.root);
    return out;
}

Node* FTree::FindParent(Node* q){
    Node *tmp=root;
    while(tmp && tmp->left!=q && tmp->right!=q){
        if(tmp->key>q->key) tmp=tmp->left;
        else tmp=tmp->right;
    }
    return tmp;
}

STree::STree(int n, bool b) {
		if (n <= 0) {
			root = NULL;
		}
		else {
            if(b){
                for (int i = 0; i < n; i++) {
                    int k;
                    cout<<"n = ";
                    cin>>k;
                    root = AddWithBalance(root, k, b);
                }
            }else{
                for (int i = 0; i < n; i++) {
                    int k = rand() % 200;
                    root = AddWithBalance(root, k, b);
                }
            }
		}

	}


Node* STree::rotate_right(Node*r, Node*p){
    Node* s = r->left;

    r->left = s->right;
    s->right = r;
    if (p) {
        if (p->left == r) {
            p->left = s;
        }
        else if (p->right == r) {
            p->right = s;
        }
    }
    else {
        root = s;
        return root;
    }

    return s;
}

Node* STree::rotate_left(Node*r, Node*p){
    Node* s = r->right;

    r->right = s->left;
    s->left = r;
    if (p) {
        if (p->left == r) {
            p->left = s;
        }
        else if (p->right == r) {
            p->right = s;
        }
    }
    else {
        root = s;
        return root;
    }

    return s;
}

Node* STree::AddWithBalance(Node*p, int k, bool &b){
    if (p == NULL) { 
			Node* p = new Node(k);
			b = true;
			return p; 
	}
    if (k < p->key){ 
        p->left = AddWithBalance(p->left, k, b);
        if (b) {
            if (p->balance == 1) {
                p->balance = 0;
                b = false;
                return  p;
            }
            else if (p->balance == 0) {
                p->balance = -1;
                return  p;
            }
            else if (p->balance == -1) {
                p->balance = -2;
                Node* s = p->left;
                if (s->balance == -1) {
                    p->balance = 0;
                    s->balance = 0;
                    p = rotate_right(p, FindParent(p));
                    
                }
                else if (s->balance == 1) {
                    Node* d = s->right;
                    if (d->balance == -1) {
                        p->balance = 1;
                        s->balance = 0;
                    }
                    else if (d->balance == 1) {
                        p->balance = 0;
                        s->balance = -1;
                    }
                    else if (d->balance == 0) {
                        p->balance = 0;
                        s->balance = 0;
                    }
                    d->balance = 0;
                    s = rotate_left(s, FindParent(s));
                    p = rotate_right(p, FindParent(p));
                    
                    p = d;
                }
                b = false;
                return p;
            }
        }
    }else{
        p->right = AddWithBalance(p->right, k, b);
        if (b) {
				if (p->balance == -1) {
					p->balance = 0;
					b = false;
					return  p;
				}
				else if (p->balance == 0) {
					p->balance = 1;
					return  p;
				}
				else if (p->balance == 1) {
					p->balance = 2;
					Node* s = p->right;
					if (s->balance == 1) {
						p->balance = 0;
						s->balance = 0;
						p = rotate_left(p, FindParent(p));
						
					}
					else if (s->balance == -1) {
						Node* d = s->left;
						if (d->balance == 1) {
							p->balance = -1;
							s->balance = 0;
						}
						else if (d->balance == -1) {
							p->balance = 0;
							s->balance = 1;
						}
						else if (d->balance == 0) {
							p->balance = 0;
							s->balance = 0;
						}
						d->balance = 0;
						s = rotate_right(s, FindParent(s));
						p = rotate_left(p, FindParent(p));
						
						p = d;
					}
					b = false;
					return p;
				}
			}
		}
		return p;
}



int main(){
    srand(time(0));
    /*FTree a(10);
    cout<<a;
    a.Add(27);
    a.Add(25);
    a.Add(37);
    cout<<endl<<endl<<a<<endl;
    a.Del(34);
    cout<<endl<<endl<<a<<endl;*/
    STree a(20,0);
    cout<<a;

}   