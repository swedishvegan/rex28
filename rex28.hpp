#ifndef _rex28_inc
#define _rex28_inc
#include <cstdint>
#include <cstring>
#include <cassert>

namespace __rex28{

template<typename T=int>struct re_T{enum class op{c,a,k,o,cc,cs};op t;bool m=false,e=false;re_T(op x):t(x){}virtual~re_T()=default;virtual void init(bool,bool)=0;virtual void step(char,bool,bool)=0;virtual re_T*cp()const=0;};using re=re_T<int>;

template<typename T=int>struct rc_T:re{re*l,*r;rc_T(re*a,re*b):re(op::c),l(a),r(b){e=l->e&r->e;}~rc_T(){delete l;delete r;}void init(bool s,bool rv)override{re*p=rv?r:l,*q=rv?l:r;p->init(s,rv);q->init(p->m,rv);m=q->m;}void step(char c,bool s,bool rv)override{re*p=rv?r:l,*q=rv?l:r;bool qs=p->m|(s&p->e);p->step(c,s,rv);q->step(c,qs,rv);m=q->m|(p->m&q->e);}re*cp()const override{return new rc_T(l->cp(),r->cp());}};using rc=rc_T<int>;

template<typename T=int>struct ra_T:re{re*l,*r;ra_T(re*a,re*b):re(op::a),l(a),r(b){e=l->e|r->e;}~ra_T(){delete l;delete r;}void init(bool s,bool rv)override{l->init(s,rv);r->init(s,rv);m=l->m|r->m;}void step(char c,bool s,bool rv)override{l->step(c,s,rv);r->step(c,s,rv);m=l->m|r->m;}re*cp()const override{return new ra_T(l->cp(),r->cp());}};using ra=ra_T<int>;

template<typename T=int>struct rk_T:re{re*r;explicit rk_T(re*n):re(op::k),r(n){e=true;}~rk_T(){delete r;}void init(bool s,bool rv)override{r->init(s,rv);m=s;}void step(char c,bool s,bool rv)override{r->step(c,s|m,rv);m=r->m;}re*cp()const override{return new rk_T(r->cp());}};using rk=rk_T<int>;

template<typename T=int>struct ro_T:re{re*r;explicit ro_T(re*n):re(op::o),r(n){e=true;}~ro_T(){delete r;}void init(bool s,bool rv)override{r->init(s,rv);m=s;}void step(char c,bool s,bool rv)override{r->step(c,s,rv);m=r->m;}re*cp()const override{return new ro_T(r->cp());}};using ro=ro_T<int>;

template<typename T=int>struct rcc_T:re{uint8_t s,h;rcc_T(uint8_t lo,uint8_t hi):re(op::cc),s(lo),h(hi){}void init(bool,bool)override{m=false;}void step(char c,bool st,bool)override{m=st&&uint8_t(c)>=s&&uint8_t(c)<=h;}re*cp()const override{return new rcc_T(s,h);} };using rcc=rcc_T<int>;

template<typename T=int>struct rcs_T:re{char*p;uint64_t x=0;uint8_t n;bool own=false;rcs_T(const char*s,uint8_t len):re(op::cs),p(const_cast<char*>(s)),n(len){if(len<2){e=(len==0);return;}const char*q=s;while(*q&&*q!='\\')++q;if(!*q)return;p=new char[len];own=true;n=0;bool b=false;for(uint8_t i=0;i<len;++i){char c=s[i];if(b){p[n++]=c;b=false;}else if(c=='\\')b=true;else p[n++]=c;}if(b)p[n++]='\\';e=(n==0);}~rcs_T(){if(own)delete[] p;}void init(bool,bool)override{m=false;x=0;}void step(char c,bool st,bool rv)override{if(!n){m=st;return;}m=false;x|=st;uint64_t bit=1ULL<<(n-1),last=bit;if((x&bit)&&c==p[rv?0:n-1])m=true;x&=~bit;for(int i=n-2;i>=0;--i){bit=1ULL<<i;if((x&bit)&&c==p[rv?n-i-1:i])x|=last;x&=~bit;last=bit;}}re*cp()const override{return new rcs_T(p,n);} };using rcs=rcs_T<int>;

template<typename T=int>struct rex_T{re*r;int pos=-1;unsigned len=0;static re*lit(char c){return new rcc(static_cast<uint8_t>(c),static_cast<uint8_t>(c));}static re*f(const char*p,uint16_t s,uint16_t e){if(!e)e=uint16_t(std::strlen(p));if(s>=e)return new rcs("",0);{int par=0;bool bs=false;for(uint16_t i=s;i<e;++i){char ch=p[i];if(!bs){if(ch=='\\'){bs=true;continue;}if(ch=='('){++par;continue;}if(ch==')'){--par;continue;}if(ch=='|'&&par==0)return new ra(f(p,s,i),f(p,i+1,e));}else bs=false;}}re*seq=nullptr;uint16_t i=s;while(i<e){re*n=nullptr;char ch=p[i];if(ch=='\\'){if(i+1>=e){n=lit('\\');++i;}else if(i+2<e&&p[i+2]=='-'){char lo=p[i+1];uint16_t j=i+3;char hi;if(j<e&&p[j]=='\\'){++j;assert(j<e);hi=p[j++];}else{assert(j<e);hi=p[j++];}n=new rcc(uint8_t(lo),uint8_t(hi));i=j;}else{n=lit(p[i+1]);i+=2;}}else if(ch=='('){uint16_t beg=++i,par=1;bool bs=false;for(;i<e;++i){char d=p[i];if(!bs){if(d=='\\'){bs=true;continue;}if(d=='('){++par;continue;}if(d==')'&&--par==0)break;}else bs=false;}assert(i<e);n=f(p,beg,i);++i;}else{if(i+2<e&&p[i+1]=='-'){char lo=ch;uint16_t j=i+2;char hi;if(p[j]=='\\'){++j;assert(j<e);hi=p[j++];}else hi=p[j++];n=new rcc(uint8_t(lo),uint8_t(hi));i=j;}else{n=lit(ch);++i;}}if(i<e){char u=p[i];if(u=='*'||u=='?'||u=='+'){if(u=='*')n=new rk(n);else if(u=='?')n=new ro(n);else{re*head=n->cp();n=new rc(head,new rk(n));}++i;}}seq=seq?static_cast<re*>(new rc(seq,n)):n;}return seq?seq:new rcs("",0);}rex_T(const char*pat){r=f(pat,0,0);}~rex_T(){delete r;}bool match(const char*s,bool rv=false){r->init(true,rv);for(int i=0;s[i];++i)r->step(s[i],i==0,rv);return r->m;}bool next(const char*s){r->init(true,true);int st=pos>=0?pos+len:0;int a=-1,b=0,i=st;while(s[i])++i;for(--i;i>=st;--i){r->step(s[i],true,true);if(r->m)a=i;}if(a<0){pos=-1;len=0;return false;}r->init(true,false);for(i=a;s[i];++i){r->step(s[i],i==a,false);if(r->m)b=i;}pos=a;len=1u+(b-a);return true;}void reset(){pos=-1;len=0;}bool first(const char*s){reset();return next(s);} };using rex=rex_T<int>;

}

using rex=__rex28::rex_T<int>;
#endif
