#include "bignum.h"
#include <string>
#include <algorithm>
using namespace std;

//prueba comentario 
bignum::bignum()
{
  signo = true;
  precision = 10;
  digits = new unsigned short[precision];

  for(size_t i=0;i<precision;i++)
  {digits[i]=0;}
}


bignum::bignum(const unsigned short a)
{
  signo = true;
  precision = a;
  digits = new unsigned short[precision];
  for(size_t i=0;i<precision;i++)
  {digits[i]=0;}
}

bignum::bignum(const string &str1)
{
  //Saco los espacios en blanco.
  string str;
  for(char c:str1) if(!isspace(c)) str += c ;
  //Defino el signo.
  if(str[0]=='-')
  {
    signo=false;
    precision=str.length()-1;
  } else
  {
    signo=true;
    precision=str.length();
  }
  //Creo el arreglo de shorts
  digits=new unsigned short[precision];
  for(size_t i=0;i<precision;i++)
  {
    digits[precision-1-i]=str[precision-signo-i]-ASCII_FIX;
  }
}

bignum::~bignum()
{
  if(digits)
  {delete[] digits;}
}

void bignum::set_p(unsigned short a)
{
  precision=a;
}

unsigned short bignum::get_p()
{
  return precision;
}
void bignum::set_signo(bool s){
  signo=s;
}

const bignum bignum::operator=(const bignum& right)
{
  if(&right !=this)
  {
    signo=right.signo;
    if(precision!=right.precision)
    {
      unsigned short *aux;
      aux=new unsigned short[right.precision];
      delete[]digits;
      precision=right.precision;
      digits=aux;
      for(int i=0; i<precision; i++)
        {digits[i]=right.digits[i];}
      return *this;
    }
    else
    {
      for(int i=0; i<precision; i++)digits[i]=right.digits[i];
      return *this;
    }
  }
  return *this;
}

const bignum& bignum::operator=(const string& right)
{
  string str;
  for(char c:right) if(!isspace(c)) str += c ;

  if(str[0]=='-'){signo=false;}
  if(precision!=str.length())
  {
    unsigned short *aux;
    aux=new unsigned short[str.length()-!signo];
    delete[]digits;
    precision=str.length()-!signo;
    digits=aux;
    for(int i=0; i<precision; i++)
      digits[precision-1-i]=str[precision-signo-i]-ASCII_FIX;
    return *this;
  }else
  {
    for(int i=0; i<precision; i++)
      digits[precision-1-i]=str[precision-signo-i]-ASCII_FIX;
    return *this;
  }
}

bignum operator+(const bignum& a, const bignum& b)
{
  unsigned short n = a.precision;
  unsigned short m = b.precision;
  bignum result(max(n,m)+1);
  if(!a.signo && b.signo){
    //a.set_signo(true);
    //return b-a;
  }
  if(a.signo && !b.signo){
    //b.set_signo(true);
    //return a-b;
  }
  if(!a.signo && !b.signo){
    result.signo=false;
  }
  unsigned short carry = 0;
  unsigned short aux = 0;

  for(size_t i=0; i< max(n,m)+1;i++){

    if((short)(m-i-1)<0 && (short)(n-i-1)<0){
      aux=carry;
    } else if((short)(n-i-1)<0){
      aux=b.digits[m-i-1]+carry;
    } else if((short)(m-i-1)<0){
      aux=a.digits[n-i-1]+carry;
    } else {
      aux=a.digits[n-i-1]+b.digits[m-i-1]+carry;
    }

    if(aux>=10){
      aux-=10;
      carry=1;
    } else {
      carry=0;
    }
    result.digits[max(n,m)-i]=aux;
    result.digits[0] = aux;
  }

  return result;
}

bignum operator-(const bignum& a, const bignum& b)
{
  unsigned short cant = max(a.precision,b.precision); 
  unsigned short resto = 0;
  int aux = 0;
  bignum result(cant); 
  int i = 0, contador_a = 0, contador_b = 0;
  unsigned int dif_a = 0, dif_b = 0;
  
  if(a.precision <= b.precision){
    dif_a = cant - a.precision;
    dif_b = 0;
  }
  else {
    dif_b = cant - b.precision;
    dif_a = 0;
  }
  
  for(i = 0; i < cant; i++){
    cout << "Iteracion nro" << i <<endl;

    contador_a = cant - 1 - dif_a - i;
    contador_b = cant - 1 - dif_b - i;

    if(contador_a >= 0 && contador_b >=0){ // para evitar que ingrese a algo que no existe
      cout << "Digito" <<a.digits[contador_a] << endl;
      aux = (int)a.digits[contador_a];
      cout << "Aux casteado:" << aux << endl;
      aux -= resto;

      //verifico si tengo que "pedir" al de al lado 
      if(aux < b.digits[contador_b]){
        aux += 10;
        resto = 1;
      }
      else resto = 0;
      //cout << "aux" << aux <<endl;
      //cout << "resto" << resto <<endl;

      //resto
      aux -= (int)b.digits[contador_b]; 
      //cout << "Rtado final" << aux <<endl;
      
      if(aux <0){ 
        result.signo=false; 
        aux = aux - (aux*2); 
      }

      result.digits[cant-i-1] = aux;
    }

    //si la precision de A es mas chica
    if(contador_a < 0 && contador_b >= 0){

      result.digits[cant-i-1] = b.digits[contador_b] - resto;
      resto=0;
      result.signo = false;
    }
    //si la precision de B es mas chica
    if(contador_b < 0 && contador_a >= 0){
      result.digits[cant-i-1] = a.digits[contador_a]- resto;
      resto=0;
      result.signo = true;
    }



    //cout << result.digits[cant-i-1] << endl;
  }
    //cout << result.digits[0] << endl;
  return result;
}


ostream& operator<<(ostream& os, const bignum& num)
{
  if(num.signo==false)
  { os << '-'; }
  bool aux= false;
  for(size_t i = 0; i< num.precision;i++)
  {
    //saco los ceros de la izquierda
    if(num.digits[i]==0 && aux==false)
    {continue;}
    os << num.digits[i];
    aux=true;
  }
  return os;
}

istream& operator>>(istream& is, bignum& num)
{
  string s;
  is >> s;
  num = s;
  return is;
}
