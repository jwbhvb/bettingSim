#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

const int STARTING_BET = 10;
const int MAX_BET = 1000;

int getRandomNum(int beg, int end);
bool checkBet(int betType, int num);
bool isRed(int num);
bool isBlack(int num);
bool isEven(int num);
bool isOdd(int num);
bool isZero(int num);
bool isDoubleZero(int num);
void runSim(int cash, int betType, int numPlayers);

int startingCash;
int playerCount;

int main()
{
  srand(time(NULL));
  cout<<"Enter starting cash: ";
  cin>>startingCash;
  cout<<"Enter number of players: ";
  cin>>playerCount;
  runSim(startingCash,0,playerCount); //black
  runSim(startingCash,1,playerCount); //red
  runSim(startingCash,2,playerCount); //even
  runSim(startingCash,3,playerCount); //odd
  runSim(startingCash,4,playerCount); //random

  system("PAUSE");
  return 0;
}

int getRandomNum(int beg, int end)
{
  int ret;
  ret = beg + rand()%(end-beg+1);
  return ret;
}

bool checkBet(int betType, int num)
{
  switch(betType)
  {
    case 0: //Black
      return isBlack(num);
      break;
    case 1: //Red
      return isRed(num);
      break;   
    case 2: //Even
      return isEven(num);
      break;
    case 3: //Odd
      return isOdd(num);
      break;
    case 4: //Zero
      return isZero(num);
      break;
    case 5: //Double Zero
      return isDoubleZero(num);
      break;
    default: //Error
      cout<<"Error"<<endl;
      exit(1);
      break;
  }
  return false;
}

bool isRed(int num)
{
  vector<int> reds(18);
  reds = {1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36};
  if(!binary_search(reds.begin(),reds.end(),num))
    return false;
  return true;
}

bool isBlack(int num)
{
  if(num==0||num==-1||isRed(num))
    return false;
  return true;
}

bool isEven(int num)
{
  if(num==0||num==-1||num%2)
    return false;
  return true;
}

bool isOdd(int num)
{
  if(num==0||num==-1||isEven(num))
    return false;
  return true;
}

bool isZero(int num)
{
  if(num==0)
    return true;
  return false;
}
bool isDoubleZero(int num)
{
  if(num==-1)
    return true;
  return false;
}

void runSim(int cash, int betType, int numPlayers)
{
  int choice=betType;
  string type;
  switch(choice)
  {
    case 0:
      type="Black";
      break;
    case 1:
      type="Red";
      break;
    case 2:
      type="Even";
      break;
    case 3:
      type="Odd";
      break;
    case 4:
      type="Random";
      break;
    case 5:
      type="Zero";
      break;
    case 6:
      type="DoubleZero";
      break;
  }
  cout<<"Starting "<<type<<" simulation for "<<numPlayers<<" players with $"<<cash<<endl;
  ofstream file("output"+type+to_string(cash)+".csv");
  
  for(int i=0;i<numPlayers;i++) //for each player
  {
    long long betAmount = STARTING_BET;
    int money = cash;
    int betLevel = 0;
    int highestAmount = cash;

    while(money>0) //while has money
    {
      if(betType==4)
        choice=rand()%4;
      if(MAX_BET >= money || STARTING_BET * (int)pow(2,betLevel) <= money) //enough money to double the bet
      {
        betAmount = (int)min(STARTING_BET * (int)pow(2,betLevel),MAX_BET);
      }else{ //not enough to make the full doubled bet
        betAmount = money;
      }
      money-=betAmount; //make bet
      int randNum = getRandomNum(-1,36); //random number from -1 to 36 where -1 = 00

      bool won = checkBet(choice, randNum);
      if(won)
      {
        if(choice<4){ //won a 2 to 1 payout (black, red, even, or odd)
          money += 2*betAmount;
        }else{ //0 and 00 payout
          money += 35*betAmount;
        }
        if(money>highestAmount)
          highestAmount=money;
        betAmount = STARTING_BET;
        betLevel = 0;
      }else{ //lost
        betLevel++;
      }
    }

    if(i==0){
      file<<highestAmount<<",AVERAGE,MAX"<<endl;
    }else if(i==1){
      file<<highestAmount<<",=AVERAGE(A:A),=MAX(A:A)"<<endl;
    }else{
      file<<highestAmount<<endl;
    }
    if(i%(numPlayers/10)==0&&i!=0)
      cout<<i/(numPlayers/10)<<"0% done"<<endl;
  }
  return;
}