#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define MAX_WORD_SIZE 100
#define MAX_MEANING_SIZE 100

typedef struct {
    char word[MAX_WORD_SIZE];
    char meaning[MAX_MEANING_SIZE];
}element;

typedef struct TreeNode {
    element key;
    struct TreeNode* left, * right;
}TreeNode;

int compare(element e1, element e2) { //��
    //e1 < e2 ? -1 : e1 == e2 ? 0: e1 > e2 1:;
    return strcmp(e1.word, e2.word);
}

void display(TreeNode* p) { //Ž�� Ʈ�� ���
    if (p != NULL) {
        printf("(");
        display(p->left);
        printf("%s", p->key.word);
        display(p->right);
        printf(")");
    }
}

//Ʈ���� ��� �����ϱ�
//key�� �̹� ������ ���Ե��� ����. (���� �� ����)
//key = node's value = e's element
void insert_node(TreeNode** roots, element key) { 
    TreeNode* parent; TreeNode* child;
    TreeNode* newNode;

    child = *roots; // ��Ʈ�� �ڽĿ���
    parent = NULL;

    //search (Ű�� ���� �ϴ���.)
    while (child != NULL) {

        //ó�� ������ child = root�̹Ƿ� root���� ����
        if (compare(key, child->key) == 0) {
            return; //���� Ű �� ���߽� ���� X
        }
        //�ƴҽ� ���� ����
        
        //Ž���� ����� �ּҸ� �θ𿡰� ���� (�ߴܵȴٸ� �� ��忡 �޾ƾ� �ϹǷ� �� ����� �θ��̱� ����)
        parent = child; //�θ𿡰� �ڽ��� �ּ� ����

        //�Ŀ� �ڽ��� �ڽ��� ���� ���� �̵��ϱ� ������ �θ� �ڽİ��谡 ����
        if (compare(key, child->key) == 1) { //���Ե� ���� �� ���� ��� (���� > Ž�� �ڷ�) -> 1
            child = child->left;  //�������� �̵�
        }
        else { //���Ե� ���� �� ū ��� (���� < Ž�� �ڷ�) -> -1
            child = child->right;  //���������� �̵�
        }

    } //Ž�� ��

    //���� Ű�� �����Ƿ� ����.
    newNode = (TreeNode*)malloc(sizeof(TreeNode)); //����ü �޸� ũ�⸸ŭ �Ҵ�.
    if (newNode == NULL) return; //���� �Ҵ� ����

    newNode->key = key;
    newNode->left = newNode->right = NULL; //���ο� ��� (���� X)

    if (parent != NULL) { //�θ� ������ ���
        if (compare(key, parent->key) == -1) { //�θ� �� ū ���
            parent->left = newNode; //�θ��� ���ʿ� ����
        }
        else { //�θ𺸴� �� ū ���
            parent->right = newNode; //�θ��� �����ʿ� ����
        }
    }
    else if (parent == NULL) { //�θ� ���� ���. 
        roots = newNode; //root�� ���� ��� �ּ� ����.
    }


}


void delete_node(TreeNode** root, element key) {
    TreeNode* parent, * childNode, * similar_child, * similar_parent, * child;

    //key�� ���� ��� child�� Ž��,  parent�� child�� �θ�
    parent = NULL;
    child = *root;

    while (child != NULL && compare(child->key, key) != 0) { //�ڽ� �ּҿ� 0�� ���� �ʰ�, ���ڿ� �񱳿��� �ٸ��� ������ ������ �ݺ�
        parent = child;
        child = (compare(key, child->key) == -1) ? child->left : child->right; //���� ���ڿ��� �� ���ڿ� ���� Ŭ ��. ����, �ƴ� ������
    }

    if (child == NULL) { //������� ���
        printf("\n ���� ���� �Դϴ�!\n");
        return;
    }

    //�����Ϸ��� �ܾ �ܸ� ��� (�� ��)�� ���
    if (child->left == NULL && child->right == NULL) {
        if (parent != NULL) { //�ܸ� ����ε� �θ� ������ ��� 
            if (parent->left == child) { //[�θ�] -> [���](��)
                parent->left = NULL; //���� ����
            }
            else {
                parent->right == NULL; //���� ����
            }
        }
        else if (parent == NULL) { //�θ� ���� ��Ȳ (���� ��尡 root�� ���
            *root == NULL; //root ��ü�� ����
        }
    }


    //�����Ϸ��� �ܾ��� �ڼ��� 1�� �����ϴ� ���
    else if ((child->left != NULL) || (child->right != NULL)) { //���� �Ǵ� �����ʿ� ��尡 �����ϴ� ��� (�ڼ��� ��� �ִ°� �ľ�
        childNode = (child->left != NULL) ? child->left : child->right;
                                           //�����Ϸ��� ����� �ڼ��� �ּҸ� ����

        if (parent != NULL) { //�����Ϸ��� ����� �θ� ����
            if (parent->left == child) { //�θ��� ���ʿ� �����Ͽ��� ��尡 ������ִٸ�
                parent->left = childNode; //���� ������ ������ �����ϰ� ���ʿ� �ڼ� ��带 ����
            }
            else { //�����ʿ� ����Ǿ��ٸ�
                parent->right = childNode; //���� ������ ������ �����ϰ� �����ʿ� �ڼ� ��带 ����
            }

            //[�θ�] -> [���� ���] -> [�ڼ�]       ->      [�θ�] ->   ->   -> [�ڼ�]
            //                                                     [���� ���]

        }
        else { //�θ� ���� ���.
            *root = NULL;
        }
    }


    //�����Ϸ��� �ܾ��� �ڼ��� �������� ���
    else {
        // ������ ����Ʈ������ �İ��ڸ� ã�´�.
        similar_parent = child; 
        similar_child = child->right; //Ž���� ���� ��ġ�� �����Ϸ��� ����� ������ �ڼ��̴�.

        // �İ��ڸ� ã�Ƽ� ��� �������� �̵��Ѵ�.
        while (similar_child->left != NULL) { //�������� �̵��� ��,NULL�� ���ö� ����(������) �ݺ�
            similar_parent = similar_child; //��� �Ʒ��� ���鼭
            similar_child = similar_child->left; //�������� ������ �̵�
                                                   //�׷��� ������ �����Ϸ��� ����� �����ٴ� ũ�� ��, ���� �ڼ��� �װͺ��� �۱� �����̴�. (���� ȸ��)
        }

        // �ļ����� �θ�� �ڽ��� ���� 
        if (similar_parent->left == similar_child) //���� �ڼտ��� �ļ��ڸ� ã�Ҵٸ�
            similar_parent->left = similar_child->right;
        else //������ �ڼտ��� �ļ��ڸ� ã�Ҵٸ� (���� �̵� ���� �� �ڸ����� �ٷ� ã���� ���)
            similar_parent->right = similar_child->right;


        //toooooo
        // �ļ��ڰ� ���� Ű���� ���� ��忡 ���� (= ���� ����� ��)
        child->key = similar_child->key;
        // ������ �ļ��� ���� (���� ��ġ�� �ļ��� ����)
        child = similar_child;
    }
    free(child);
}

void titleMenu() {
    printf("\n\n\n#####MENU#####\n");
      printf("#  i.  �Է�  #\n");
      printf("#  d.  ����  #\n");
      printf("#  s.  Ž��  #\n");
      printf("#  p.  ���  #\n");
      printf("#  q.  ����  #\n");
      printf("##############\n");
      printf("��� : ");

}

void gets_(char* msg) {
    
    gets(msg);
    
}

void main() {
    char cmd;

    element e;
    TreeNode* root = NULL;
    TreeNode* tmp;

    do {
        titleMenu();
        fflush(stdin); //�Է� ���� ������ �޸� ����
        cmd = getchar();

        switch (cmd) {
            case 'i': 
                printf("\n\n�Է�\n �ܾ� : ");
                fflush(stdin); scanf_s("%s", e.word);
                printf("\n �ǹ� : ");
                fflush(stdin); scanf_s("%s", e.meaning);
                insert_node(&root, e);
                break;

            case 'd': break;
            case 's': break;
            case 'p': break;
            case 'q': exit(1); break;
        }
    } while (1);
    
}
