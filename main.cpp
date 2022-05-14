#include <iostream>
#include <fstream>

using namespace std;

ifstream in("input.in");

struct MinHeapNode {
    char Character;
    int Frequency;
    MinHeapNode *Left, *Right;
};

class MinHeap {
    private:
        int size;
        const int capacity = 10000;
        MinHeapNode **Array;
        int LeftChild(int x) const {
            return 2 * x;
        }
        int RightChild(int x) const {
            return 2 * x + 1;
        }
        int Father(int x) const {
            return x/2;
        }
        void Heapify_Up(int child)
        {
            int father = Father(child);
            if(father > 0 and Array[father]->Frequency > Array[child]->Frequency)
            {
                swap(Array[father], Array[child]);
                Heapify_Up(father);
            }
        }
        void Heapify_Down(int father)
        {
            int smallest_element = father, left_child = LeftChild(father), right_child = RightChild(father);
            if(left_child <= size and Array[left_child]->Frequency < Array[smallest_element]->Frequency)
                smallest_element = left_child;
            if(right_child <= size and Array[right_child]->Frequency < Array[smallest_element]->Frequency)
                smallest_element = right_child;
            if(smallest_element != father)
                swap(Array[father], Array[smallest_element]), Heapify_Down(smallest_element);
        }
    public:
        MinHeap()
        {
            size = 0;
            Array = new MinHeapNode *[capacity];
        }
        ~MinHeap()
        {
            delete []Array;
        }
        void Insert(int x, char c=0)
        {
            size ++;
            if(size >= capacity)
            {
                MinHeapNode **temp_array = new MinHeapNode *[size + capacity];
                for(int i = 1; i < size; i++)
                {
                    temp_array[i] = Array[i];
                }
                delete []Array;
                Array = temp_array;
            }
            Array[size] = new MinHeapNode;
            Array[size]->Frequency = x;
            Array[size]->Character = c;
            Array[size]->Left = NULL;
            Array[size]->Right = NULL;
            Heapify_Up(size);
        }
        void Insert(MinHeapNode * & p)
        {
            size ++;
            if(size >= capacity)
            {
                MinHeapNode **temp_array = new MinHeapNode *[size + capacity];
                for(int i = 1; i < size; i++)
                {
                    temp_array[i] = Array[i];
                }
                delete []Array;
                Array = temp_array;
            }
            Array[size] = p;
            Heapify_Up(size);
        }
        MinHeapNode* MIN() const {
            return Array[1];
        }
        void Pop()
        {
            swap(Array[1], Array[size]);
            size--;
            Heapify_Down(1);
        }
        int Size() const {
            return size;
        }
        bool Empty() const {
            return size == 0;
        }
};

class HuffmanTree {
    private:
        MinHeapNode *Root;
        MinHeap Heap;
        string Codes[256];
        void RLR(MinHeapNode *p)
        {
            cout << p->Frequency << '\n';
            if(p->Left != NULL)
                RLR(p->Left);
            if(p->Right != NULL)
                RLR(p->Right);
        }
        void RecursiveSearch(MinHeapNode *p, string code)
        {
            if(p->Left == NULL and p->Right == NULL)
            {
                Codes[int(p->Character)] = code;
            }
            else
            {
                if(p->Left!= NULL)
                    RecursiveSearch(p->Left, code + '0');
                if(p->Right != NULL)
                    RecursiveSearch(p->Right, code + '1');
            }
        }
        void CreateCodes()
        {
            RecursiveSearch(Root, "");
        }
    public:
        HuffmanTree() {}
        HuffmanTree(pair < int, char > a[], int n)
        {
            for(int i = 1; i <= n; i++)
                Heap.Insert(a[i].first, a[i].second);
        }
        ~HuffmanTree() {}
        void CreateEncoding()
        {
            MinHeapNode *p, *q, *root;
            while(Heap.Size() > 1)
            {
                p = Heap.MIN();
                Heap.Pop();
                q = Heap.MIN();
                Heap.Pop();
                root = new MinHeapNode;
                root->Left = p;
                root->Right = q;
                root->Frequency = p->Frequency + q->Frequency;
                Heap.Insert(root);
            }
            Root = Heap.MIN();
            CreateCodes();
        }
        string DecodeMessage(string encoded_message)
        {
            string message = "";
            MinHeapNode *pointer = Root;
            int n = encoded_message.length();
            for(int i = 0; i < n; i++)
            {
                if(encoded_message[i] == '0')
                    pointer = pointer->Left;
                else
                    pointer = pointer->Right;
                if(pointer->Left == NULL and pointer->Right == NULL)
                    message = message + pointer->Character, pointer = Root;
            }
            return message;
        }
        string EncodeMessage(string message)
        {
            string encoded_message = "";
            int n = message.length();
            for(int i = 0; i < n; i++)
                encoded_message = encoded_message + Codes[int(message[i])];
            return encoded_message;
        }
};

int main()
{
    char c;
    string s;
    int x, n = 0;
    pair < int, char > a[256];
    while(in >> c >> x)
        a[++n].first = x, a[n].second = c;
    HuffmanTree tree(a, n);
    tree.CreateEncoding();
    cout << "Type the wanted option\n1.Encode message\n2.Decode message\n3.Exit\n";
    while(true)
    {
        cin >> x;
        switch (x)
        {
            case 1:
            {
                cout << "Type the message that you want to encode\n";
                cin.get();
                getline(cin, s);
                cout << tree.EncodeMessage(s) << '\n';
                break;
            }
            case 2:
            {
                cout << "Type the message that you want to decode\n";
                cin.get();
                getline(cin, s);
                cout << tree.DecodeMessage(s) << '\n';
                break;
            }
            case 3:
            {
                return 0;
            }
        }
    }
    return 0;
}
