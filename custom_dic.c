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

int compare(element e1, element e2) { //비교
    //e1 < e2 ? -1 : e1 == e2 ? 0: e1 > e2 1:;
    return strcmp(e1.word, e2.word);
}

void display(TreeNode* p) { //탐색 트리 출력
    if (p != NULL) {
        printf("(");
        display(p->left);
        printf("%s", p->key.word);
        display(p->right);
        printf(")");
    }
}

//트리에 노드 삽입하기
//key가 이미 있으면 삽입되지 않음. (유일 값 유지)
//key = node's value = e's element
void insert_node(TreeNode** roots, element key) { 
    TreeNode* parent; TreeNode* child;
    TreeNode* newNode;

    child = *roots; // 루트를 자식에게
    parent = NULL;

    //search (키가 존재 하는지.)
    while (child != NULL) {

        //처음 구동시 child = root이므로 root부터 시작
        if (compare(key, child->key) == 0) {
            return; //같은 키 값 적발시 삽입 X
        }
        //아닐시 삽입 시작
        
        //탐색한 노드의 주소를 부모에게 전달 (중단된다면 그 노드에 달아야 하므로 새 노드의 부모이기 때문)
        parent = child; //부모에게 자식의 주소 전달

        //후에 자식은 자신의 접속 노드로 이동하기 때문에 부모 자식관계가 성립
        if (compare(key, child->key) == 1) { //삽입될 것이 더 적은 경우 (삽입 > 탐색 자료) -> 1
            child = child->left;  //왼쪽으로 이동
        }
        else { //삽입될 것이 더 큰 경우 (삽입 < 탐색 자료) -> -1
            child = child->right;  //오른쪽으로 이동
        }

    } //탐색 끝

    //동일 키가 없으므로 삽입.
    newNode = (TreeNode*)malloc(sizeof(TreeNode)); //구조체 메모리 크기만큼 할당.
    if (newNode == NULL) return; //동적 할당 에러

    newNode->key = key;
    newNode->left = newNode->right = NULL; //새로운 노드 (연결 X)

    if (parent != NULL) { //부모가 존재할 경우
        if (compare(key, parent->key) == -1) { //부모가 더 큰 경우
            parent->left = newNode; //부모의 왼쪽에 연결
        }
        else { //부모보다 더 큰 경우
            parent->right = newNode; //부모의 오른쪽에 연결
        }
    }
    else if (parent == NULL) { //부모가 없는 경우. 
        roots = newNode; //root에 생성 노드 주소 삽입.
    }


}


void delete_node(TreeNode** root, element key) {
    TreeNode* parent, * childNode, * similar_child, * similar_parent, * child;

    //key를 갖는 노드 child를 탐색,  parent는 child의 부모
    parent = NULL;
    child = *root;

    while (child != NULL && compare(child->key, key) != 0) { //자식 주소에 0이 오지 않고, 문자열 비교에서 다름이 나오는 동안은 반복
        parent = child;
        child = (compare(key, child->key) == -1) ? child->left : child->right; //현재 문자열이 비교 문자열 보다 클 때. 왼쪽, 아님 오른쪽
    }

    if (child == NULL) { //비어있을 경우
        printf("\n 공백 상태 입니다!\n");
        return;
    }

    //삭제하려는 단어가 단말 노드 (맨 끝)인 경우
    if (child->left == NULL && child->right == NULL) {
        if (parent != NULL) { //단말 노드인데 부모가 존재할 경우 
            if (parent->left == child) { //[부모] -> [노드](왼)
                parent->left = NULL; //연결 해제
            }
            else {
                parent->right == NULL; //연결 해제
            }
        }
        else if (parent == NULL) { //부모가 없는 상황 (삭제 노드가 root인 경우
            *root == NULL; //root 자체를 해제
        }
    }


    //삭제하려는 단어의 자손이 1개 존재하는 경우
    else if ((child->left != NULL) || (child->right != NULL)) { //왼쪽 또는 오른쪽에 노드가 존재하는 경우 (자손이 어디 있는가 파악
        childNode = (child->left != NULL) ? child->left : child->right;
                                           //삭제하려는 노드의 자손의 주소를 보관

        if (parent != NULL) { //삭제하려는 노드의 부모가 존재
            if (parent->left == child) { //부모의 왼쪽에 삭제하여는 노드가 연결되있다면
                parent->left = childNode; //기존 노드와의 연결을 삭제하고 왼쪽에 자손 노드를 연결
            }
            else { //오른쪽에 연결되었다면
                parent->right = childNode; //기존 노드와의 연결을 삭제하고 오른쪽에 자손 노드를 연결
            }

            //[부모] -> [삭제 노드] -> [자손]       ->      [부모] ->   ->   -> [자손]
            //                                                     [삭제 노드]

        }
        else { //부모가 없는 경우.
            *root = NULL;
        }
    }


    //삭제하려는 단어의 자손이 여러개인 경우
    else {
        // 오른쪽 서브트리에서 후계자를 찾는다.
        similar_parent = child; 
        similar_child = child->right; //탐색의 시작 위치는 삭제하려는 노드의 오른쪽 자손이다.

        // 후계자를 찾아서 계속 왼쪽으로 이동한다.
        while (similar_child->left != NULL) { //왼쪽으로 이동할 시,NULL이 나올때 까지(끝까지) 반복
            similar_parent = similar_child; //계속 아래로 가면서
            similar_child = similar_child->left; //왼쪽으로 끝까지 이동
                                                   //그러는 이유는 삭제하려는 노드의 값보다는 크고 또, 왼쪽 자손은 그것보다 작기 때문이다. (원점 회귀)
        }

        // 후속자의 부모와 자식을 연결 
        if (similar_parent->left == similar_child) //왼쪽 자손에서 후속자를 찾았다면
            similar_parent->left = similar_child->right;
        else //오른쪽 자손에서 후속자를 찾았다면 (왼쪽 이동 전에 그 자리에서 바로 찾았을 경우)
            similar_parent->right = similar_child->right;


        //toooooo
        // 후속자가 가진 키값을 현재 노드에 복사 (= 가장 비슷한 값)
        child->key = similar_child->key;
        // 원래의 후속자 삭제 (삭제 위치에 후속자 연결)
        child = similar_child;
    }
    free(child);
}

void titleMenu() {
    printf("\n\n\n#####MENU#####\n");
      printf("#  i.  입력  #\n");
      printf("#  d.  삭제  #\n");
      printf("#  s.  탐색  #\n");
      printf("#  p.  출력  #\n");
      printf("#  q.  종료  #\n");
      printf("##############\n");
      printf("명령 : ");

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
        fflush(stdin); //입력 오류 방지용 메모리 비우기
        cmd = getchar();

        switch (cmd) {
            case 'i': 
                printf("\n\n입력\n 단어 : ");
                fflush(stdin); scanf_s("%s", e.word);
                printf("\n 의미 : ");
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
