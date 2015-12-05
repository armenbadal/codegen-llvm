# LLVM IR կոդի գեներերացիա

## Աբստրակտ քերականական ծառ

Աբստրակտ քերականական ծառը կազմված է չորս հանգույցների չորս
հիմնական տիպերից․ _մոդուլ_ (՝Module՝), _ֆունկցիա_ (՝Function՝),
_հրաման_ (՝Statement՝) և _արտահայտություն_ (՝Expression՝)։ 

### Մոդուլ

Մոդուլը _կոմպիլյացիայի միավորն_ է, որ պարունակում է ֆունկցիաների հայտարարությունների ու սահմանումների շարք։ `ast` անունների տիրույթի `Module` դասը մոդելավորում է կոմպիլյացիայի միավորը։

````
class Module {
private:
  std::string name; // մոդուլի անունը
  std::Vector<Function*> funcs; // ֆունկցիաներ
public:
  Module(const std::string& n)
    : name{n}
  {}
  void addFunction(Function* f)
  { funcs.push_back(f); }
  llvm::Module* code();
};
````

Այս դասի `code()` մեթոդը LLVM IR կոդ է գեներացնում ամբողջ մոդուլի համար, և վերադարձնում է կառուցված `llvm::Module` օբյեկտի ցուցիչը։


### Ֆունկցիա

Աբստրակտ քերականակակն ծառի երկրորդ խոշոր միավոր _ֆունկցիան_ է։ `ast` անունների տիրույթի `Function` դասով մոդելավորվում են և՛ ֆունկցիաները, և՛ պրոցեդուրաները։ 


### Հրամաններ

````
Statement
  Sequence
  Declare
  Assign
  Return
  If
  While
  For
  Input
  Print
````


### Արտահայտություններ

````
Expression
  Binary
  Unary
  FunCall
  Constant
  Variable
````

