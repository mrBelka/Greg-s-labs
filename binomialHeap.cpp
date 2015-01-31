#include <stdio.h>
#include <list>
#include <iostream>
#define inf 1000000000
using namespace std;


struct P_List{
	P_List* par,*c,*s;
	int degree,data;
};

void LINK(P_List* y, P_List* z){
	y->par = z;
	y->s = z->c;
	z->c = y;
	z->degree++;
}

void PASS(P_List* u){
	while(u!=NULL){
		printf("%d ",u->data);
		if(u->c!=NULL){
			PASS(u->c);
		}
		u = u->s;
	}
}

P_List* UNION(P_List* H1, P_List* H2){
	if(H1==NULL)
		return H2;
	if(H2==NULL)
		return H1;

	P_List* q;
	/*cout << endl << "begin H1" << endl;
	q = H1;
	while(q!=NULL){
		cout << q->data << endl;
		q = q->s;
	}
	cout << endl << "end H1" << endl;

	cout << endl << "begin H2" << endl;
	q = H2;
	while(q!=NULL){
		cout << q->data << endl;
		q = q->s;
	}
	cout << endl << "end H2" << endl;*/

	bool first = true;
	//составляем единый список
	P_List* _H = new P_List;
	_H->s = NULL;
	_H->par = NULL;
	_H->c = NULL;
	
	while(H1!=NULL && H2!=NULL){
		//cout << H1->data << " " << H2->data << endl;
		if(H1->degree>H2->degree){
			q = H1->s;
			H1->s = _H->s;
			_H->s = H1;
			H1 = q;
		}
		else{ 
			q = H2->s;
			H2->s = _H->s;
			_H->s = H2;
			H2 = q;	
		}
	}

	//H = H->s;
	/*cout << endl << "begin H" << endl;
	
	q = H;
	while(q!=NULL){
		cout << q->data << endl;
		q = q->s;
	}
	cout << endl << "end" << endl;*/

	while(H1!=NULL){
		q = H1->s;
		H1->s = _H->s;
		_H->s = H1;
		H1 = q;
	}
	while(H2!=NULL){
		q = H2->s;
		H2->s = _H->s;
		_H->s = H2;
		H2 = q;
	}

	P_List* H = new P_List;
	P_List* qq;
	H->s = NULL;
	q = _H->s;
	while(q!=NULL){
		qq = q->s;
		q->s = H->s;
		H->s = q;
		q = qq;
	}

	H = H->s;

	/*cout << endl << "begin H" << endl;
	
	q = H;
	while(q!=NULL){
		cout << q->data << endl;
		q = q->s;
	}
	cout << endl << "end" << endl;*/

	P_List* prev = NULL;
	P_List* p = H;
	P_List* next = p->s;
	while(next!=NULL){
		if(p->degree!=next->degree || 
			(next->s!=NULL && next->s->degree==p->degree)){
			prev = p;
			p = next;
		}
		else if(p->data<=next->data){
				p->s = next->s;
				LINK(next,p);
		}
		else {
			if(prev==NULL){
				H = next;
			}
			else{
				prev->s = next;
			}
			LINK(p,next);
			p = next;
		}
		next = p->s;
	}
	return H;
}

void ADD(int x, P_List*& H){
	P_List* v = new P_List;
	v->data = x;
	v->par = NULL;
	v->c = NULL;
	v->s = NULL;
	v->degree = 0;
	H = UNION(H,v);
}

bool SEARCH(P_List* H,int k,P_List*& adr){
	while(H!=NULL){
		if(H->data==k){
			adr = H;
			return true;
		}
		if(H->data<k && H->c!=NULL){
			if(SEARCH(H->c,k,adr))
				return true;
		}
		H = H->s;
	}
	return false;
}

void DECREASE_KEY(P_List*& adr,int k){
	adr->data = k;
	P_List* y = adr;
	P_List* z = y->par;
	while(z!=NULL && y->data<z->data){
		swap(y->data,z->data);
		swap(y->degree,z->degree);
		y = z;
		z = y->par;
	}
}

P_List* MINIMUM(P_List* H){
	int min = inf;
	P_List* adr = NULL;
	while(H!=NULL){
		if(min>H->data){
			min = H->data;
			adr = H;	
		}
		H = H->s;
	}
	return adr;
}

void EXTRACT_MIN(P_List*& H,int& x){
	if(H==NULL)
		return;
	P_List* adr = MINIMUM(H);
	x = adr->data;
	if(adr==H)
		H = H->s;
	else {
		P_List* q = H;
		while(q->s!=NULL){
			if(q->s->data==x){
				q->s = q->s->s;
				break;
			}
			q = q->s;
		}
	}

	P_List* h = NULL;
	P_List* next;
	P_List* v = adr->c;
	while(v!=NULL){
		next = v->s;
		v->s = h;
		v->par = NULL;
		h = v;
		v = next;
	}
	H = UNION(H,h);
}

void DELETE(P_List*& H,P_List*& adr,int x){
	DECREASE_KEY(adr,-inf);
	EXTRACT_MIN(H,x);
}

int main(){
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	char c;
	int x,y;
	P_List* H,*adr;
	
	H = new P_List;
	H = NULL;

	bool res;
	while(!feof(stdin)){
		scanf("%c",&c);
		//cout << endl << "op " << c << endl;
		switch(c){
			case 'E':return 0;
			case '+':scanf("%d\n",&x);
				     //cout << "ADD " << x << endl;
					 if(!SEARCH(H,x,adr))
						 ADD(x,H);
					 break;
			case '?':scanf("%d\n",&x);
					 //cout << "SEARCH " << x << endl;
					 res = SEARCH(H,x,adr);
					 printf("%c ",res==true?'y':'n');
					 break;
			case '-':scanf("%d\n",&x);
				     //cout << "DELETE " << x << endl;
				     if(SEARCH(H,x,adr))
						 DELETE(H,adr,x);
					 break;
			case 'd':scanf("%d %d\n",&x,&y);
				    //cout << "DECRKEY " << x << " " << y << endl;
				    if(SEARCH(H,x,adr))
						 DECREASE_KEY(adr,y);
					 break;
			case 'm':if(H==NULL)printf("- ");
					 else{
						 EXTRACT_MIN(H,x);
						 printf("%d ",x);
					 }
					 break;
		}
		//cout  << "end_op" << endl;
		//cout << "begin" << endl;
		//PASS(H);
		//cout << endl << "end" << endl;
	}
	return 0;
}
