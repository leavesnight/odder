#include <iostream>
#include <math.h>
#include <time.h>
#include <string>

#define insert_in(a,b,c) ((a)<(b)?b:(a)>(c)?c:a)
#define minab(a,b) ((a)>(b)?b:a)
#define maxab(a,b) ((a)<(b)?b:a)

using namespace std;

#define PI 3.1415926
#define MAX_SCORE 8000

int g_nCostSum[5][10];
const int nCostCoeff[7]={2,2,2,2,2,2,1};
const char nStraBase[7]={15,15,10,15,15,15,15};
double dWeight[5]={300,300,100,35,800};//suppose rec<=50, spec>=400
//double dWeight[5]={300,300,200,200,1500};
double dWeightMap[5][7];
int g_nMaxSet[7][5],g_nMaxValue,g_nMaxLeft;
int nAllNum=15;

void find(const int n[8],int nSet[7][5],int nStep,int nType,double nValue,int nLeft,bool bEnum[7][5]){
	int nTmp=nSet[nType][nStep];//=0
	//bEnum[nType][nStep]=true;
	int nNextStep=nStep,nNextType=nType;
	do{
		if (nNextStep==0) nNextStep=3;
		else if (nNextStep==4){
			nNextType--;
			nNextStep=0;
		}else if (nNextStep!=1) nNextStep--;
		else nNextStep=4;
	}while (nNextType>-1&&bEnum[nNextType][nNextStep]);
	if (nNextType>-1){
		find(n,nSet,nNextStep,nNextType,nValue,nLeft,bEnum);
	}
	for (int j=nSet[nType][nStep];j<10;j++){
		//cout<<"4";
		if (nStep==4){
			//cout<<"2";
			if (j>2) break;
		}
		if (nLeft<g_nCostSum[nStep][j]/nCostCoeff[nType]){
			//cout<<"1";
			if (g_nMaxValue<nValue){
				g_nMaxValue=nValue;
				g_nMaxLeft=nLeft;
				for (int i=0;i<7;i++)
					for (int j=0;j<5;j++)
						g_nMaxSet[i][j]=nSet[i][j];
			}
			break;
		}else{
			if (nStep==4)
				if (nSet[nType][0]+nSet[nType][1]+nSet[nType][2]+nSet[nType][3]>=nStraBase[nType]+10*j&&nSet[6][0]+nSet[6][1]+nSet[6][2]+nSet[6][3]>=15+10*j)
					goto label1;
				else break;
			else if (nStep==0)
				if (nType==6||j<2||nSet[6][0]+nSet[6][1]+nSet[6][2]+nSet[6][3]>=15+10*((j-2)/3))
					goto label1;
				else break;
			else if (nSet[nType][(nStep+1)%4]>=((j+1)/3)*3){
				label1:
				//if (nType==0) cout<<"3";
				nSet[nType][nStep]=j+1;
				if (nNextType>-1){
					find(n,nSet,nNextStep,nNextType,nValue+(j+1)*dWeight[nStep]/nAllNum*n[nType],nLeft-g_nCostSum[nStep][j]/nCostCoeff[nType],bEnum);
				}else{
					if (g_nMaxValue<nValue+(j+1)*dWeight[nStep]/nAllNum*n[nType]){
						g_nMaxValue=nValue+(j+1)*dWeight[nStep]/nAllNum*n[nType];
						g_nMaxLeft=nLeft-g_nCostSum[nStep][j]/nCostCoeff[nType];
						for (int i=0;i<7;i++)
							for (int j=0;j<5;j++)
								g_nMaxSet[i][j]=nSet[i][j];
					}
				}
			}else break;
		}
	}
	nSet[nType][nStep]=nTmp;
	//bEnum[nType][nStep]=false;
}
//int f[MAX_SCORE/5+1][40+1][40+1][4];//occupation;the points' sum of the full type;the points' sum of the single type;the order of the former step
int f[7][5][MAX_SCORE/5+1][40+1][40+1][4];

int main(){
	using namespace std;
	cout<<"Input your numbers of different type following the order of light,dark,fire,thunder,water,wood:"<<endl;
	int n[8];
	n[6]=0;
	for (int i=0;i<6;i++)
		cin>>n[i];
	for (int i=0;i<6;i++){
		n[6]+=n[i];
	}
	if (n[6]==0) n[6]=15;//n[6] means the whole numbers, n[7] means the room; but if the user hasn't input anything n[6] defaults to 15
	nAllNum=n[6];
	cout<<"You have input the number order as: "<<endl;
	for (int i=0;i<6;i++)
		cout<<n[i]<<" ";
	cout<<endl;

	cout<<"Input your numbers of tank with different type following the same order:(the left is dps)"<<endl;
	int n2[8];
	n2[6]=0;
	for (int i=0;i<6;i++)
		cin>>n2[i];
	for (int i=0;i<6;i++){
		n2[6]+=n2[i];
	}
	if (n2[6]==0) n2[6]=15;
	cout<<"You have input the number order as: "<<endl;
	for (int i=0;i<6;i++)
		cout<<n2[i]<<" ";
	cout<<endl;
	
	dWeight[0]=300;dWeight[1]=400;dWeight[2]=70;dWeight[3]=25;dWeight[4]=1000;
	cout<<"Then input your achievement score: "<<endl;
	cin>>n[7];
	cout<<"You have input the score: "<<n[7]<<endl;
	cout<<"Then input your dps weight with the order of vit,atk,def,rec,spec(all should be an integer near 100, for example 300,400,70,25,1000,"
		" if you want to directly use the default example, please enter the enter key!): "<<endl;
	cin.get();
	char ch1=cin.get();
	if (ch1!='\n'){
		dWeight[0]=0;
		do{
			dWeight[0]=dWeight[0]*10+ch1-'0';
			ch1=cin.get();
		}while (ch1!='\n'&&ch1!=' ');
		ch1='0';
		for (int i=1;i<5;i++){
			cin>>dWeight[i];
		}
		cin.get();
		cout<<"You have input the weight: ";
		for (int i=0;i<5;i++){
			cout<<dWeight[i]<<" ";
		}
		cout<<endl;
	}

	for (int i=0;i<7;i++){
		for (int j=0;j<5;j++){
			dWeightMap[j][i]=dWeight[j]*(n[i]-n2[i])/nAllNum;
		}
	}

	dWeight[0]=300;dWeight[1]=150;dWeight[2]=150;dWeight[3]=50;dWeight[4]=500;
	cout<<"Then input your tank weight with the order of vit,atk,def,rec,spec(all should be an integer near 100, for example 300,150,150,50,500,"
		" if you want to directly use the default example, please enter the enter key!): "<<endl;
	ch1=cin.get();
	if (ch1!='\n'){
		dWeight[0]=0;
		do{
			dWeight[0]=dWeight[0]*10+ch1-'0';
			ch1=cin.get();
		}while (ch1!='\n'&&ch1!=' ');
		ch1='0';
		for (int i=1;i<5;i++){
			cin>>dWeight[i];
		}
		cout<<"You have input the weight: ";
		for (int i=0;i<5;i++){
			cout<<dWeight[i]<<" ";
		}
		cout<<endl;
	}

	for (int i=0;i<7;i++){
		for (int j=0;j<5;j++)
			dWeightMap[j][i]+=dWeight[j]*n2[i]/nAllNum;
	}

	const int nCost[4][10]=
	{20,40,40,60,80,80,100,120,120,140,
	60,120,120,180,240,240,300,360,360,420,
	20,40,40,60,80,80,100,120,120,140,
	10,20,20,30,40,40,50,60,60,70};
	const int nCost2[3]={200,400,600};
	for (int i=0;i<4;i++){
		g_nCostSum[i][0]=nCost[i][0];
		for (int j=1;j<10;j++)
			g_nCostSum[i][j]=g_nCostSum[i][j-1]+nCost[i][j];
	}
	g_nCostSum[4][0]=nCost2[0];
	for (int j=1;j<3;j++)
		g_nCostSum[4][j]=g_nCostSum[4][j-1]+nCost2[j];

	int nLeft=n[7];
	int nSet[7][5]={0};	//nSet[6][x] is the full type
	double dValue[7][5]={0}; //nValue[6][x] is the full type
	/*while (1){
		double dMax=0,dMax2=0;
		int nMaxCost=0;
		for (int i=0;i<7;i++){
			for (int j=0;j<4;j++){
				if (nSet[i][j]<10&&nLeft>=(nCost[j][nSet[i][j]]/nCostCoeff[i])){
					dValue[i][j]=dWeight[j]/(nCost[j][nSet[i][j]]/nCostCoeff[i])/nAllNum*n[i];
					if (dValue[i][j]>dMax){
						dMax2=dMax;dMax=dValue[i][j];nMaxCost=(nCost[j][nSet[i][j]]/nCostCoeff[i]);
					}else if (dValue[i][j]==dMax) nMaxCost+=(nCost[j][nSet[i][j]]/nCostCoeff[i]);
				}
			}
			if (nSet[i][4]<3&&nLeft>=(nCost2[nSet[i][4]]/nCostCoeff[i])){
				dValue[i][4]=dWeight[4]/(nCost2[nSet[i][4]]/nCostCoeff[i])/nAllNum*n[i];
				if (dValue[i][4]>dMax){
					dMax2=dMax;dMax=dValue[i][4];nMaxCost=(nCost2[nSet[i][4]]/nCostCoeff[i]);
				}else if (dValue[i][4]==dMax) nMaxCost+=(nCost2[nSet[i][4]]/nCostCoeff[i]);
			}
		}
		if (nMaxCost>0&&nLeft>=nMaxCost){
			for (int i=0;i<7;i++){
				for (int j=0;j<4;j++){
					if (nLeft>=(nCost[j][nSet[i][j]]/nCostCoeff[i])&&
						dValue[i][j]==dMax){
							if ()
							nSet[i][j]++;
					}
				}
				if (nLeft>=(nCost2[nSet[i][4]]/nCostCoeff[i])&&
					dValue[i][4]==dMax) nSet[i][4]++;
			}
			nLeft-=nMaxCost;
		}else break;
	}*/
	bool bEnum[7][5]={0};
	for (int i=0;i<6;i++)
		if (n[i]==0) 
			for (int j=0;j<5;j++) bEnum[i][j]=true;
	//find(n,nSet,0,6,0,n[7],bEnum);
	for (int i=6;i>-1;i--){
		int j=0;//35 steps
		do{
			/*basic unrestricted method:
			for (int k=0;k<10;k++){//1+10 selections
				if (j==4&&k>2){
					break;
				}
				if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0){
					int nTmp=0;
					nTmp=f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][0][0][0]+(k+1)*dWeight[j]/nAllNum*n[i];
					if (f[v][0][0][0]<nTmp) f[v][0][0][0]=nTmp;
				}
			}
			*/
			/*if (i!=6&&j==0){//[i-1]..[0(i-1)] is different from [i]..[0(i)]!!!we want to use [i]..[0(i)]!!!
				for (int v=n[7]/5;v>0;v--)
					for (int v2=40;v2>-1;v2--)
						for (int v3=40;v3>-1;v3--)
							for (int kk=0;kk<4;kk++){//if k=-1 only this state need to be compared with former f[v][v2][v3][x]
								if (f[v][v2][0][0]<f[v][v2][v3][kk]) f[v][v2][0][0]=f[v][v2][v3][kk];
								if (v3!=0||kk!=0) f[v][v2][v3][kk]=0;
							}
			}
			for (int v=n[7]/5;v>0;v--){
				if (i==6){
					if (j==0){
						for (int k=0;k<10;k++)//11 selections
							if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0){
								int nTmp=(k+1)*dWeight[j]/nAllNum*n[i];
								if (f[v][k+1][k+1][(k+1)/3]<nTmp) f[v][k+1][k+1][(k+1)/3]=nTmp;
							}
					}else{
						for (int v2=40;v2>0;v2--){
							for (int kk=1;kk<4;kk++){//if k=-1 only this state need to be compared with former f[v][v2][v2][x]
								if (f[v][v2][v2][0]<f[v][v2][v2][kk]) f[v][v2][v2][0]=f[v][v2][v2][kk];
								f[v][v2][v2][kk]=0;
							}
							for (int k=0;k<10;k++){//1+10 selections
								if (j==4&&k>2) break;
								if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0&&
									(j!=4&&v2-(k+1)>=0||
									j==4&&v2>=15+k*10)){
										int nMax=0;
										if (j!=4){
											for (int kk=(k+1)/3;kk<4;kk++){
												int nTmp=0;
												if (k<2&&v2==k+1||f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2-(k+1)][v2-(k+1)][kk]>0)//we must ensure the former one==0 exists only under nV2==nV3==0!!!
													nTmp=f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2-(k+1)][v2-(k+1)][kk]+(k+1)*dWeight[j]/nAllNum*n[i];
												if (nMax<nTmp) nMax=nTmp;
											}
										}else{
											for (int kk=0;kk<4;kk++){
												int nTmp=0;
												if (f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v2][kk]>0)
														nTmp=f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v2][kk]+(k+1)*dWeight[j]/nAllNum*n[i];
												if (nMax<nTmp) nMax=nTmp;
											}
										}
										if (f[v][v2][v2][(k+1)/3]<nMax) f[v][v2][v2][(k+1)/3]=nMax;
								}
							}
						}
					}
				}else{//i!=6
					if (j==0){
						for (int v2=40;v2>-1;v2--){
							for (int v3=40;v3>-1;v3--){
								for (int k=0;k<10;k++){//11 selections
									if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0&&
										v3==k+1&&(k<2||v2>=15+((k-2)/3)*10)){
											int nMax=0;
											for (int kk=0;kk<4;kk++){
												int nTmp=0;
												if (k<2&&v2==0||f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][0][kk]>0)//[i-1]..[0(i-1)] is different from [i]..[0(i)]!!!we want to use [i]..[0(i)], but it doesn't mean we can take twice or more! so we cannot miss the order of v!
													nTmp=f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][0][kk]+(k+1)*dWeight[j]/nAllNum*n[i];
												if (nMax<nTmp) nMax=nTmp;
											}
											if (f[v][v2][k+1][(k+1)/3]<nMax) f[v][v2][k+1][(k+1)/3]=nMax;
									}
								}
							}
						}
					}else{
						for (int v2=40;v2>-1;v2--){
							for (int v3=40;v3>-1;v3--){
								for (int kk=1;kk<4;kk++){//if k=-1 only this state need to be compared with former f[v][v2][v3][x]
									if (f[v][v2][v3][0]<f[v][v2][v3][kk]) f[v][v2][v3][0]=f[v][v2][v3][kk];
									f[v][v2][v3][kk]=0;
								}
								for (int k=0;k<10;k++){//1+10 selections
									if (j==4&&k>2) break;
									if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0&&
										(j!=4&&v3-(k+1)>=0||
										j==4&&v2>=15+k*10&&v3>=nStraBase[i]+k*10)){
											int nMax=0;
											if (j!=4){
												for (int kk=(k+1)/3;kk<4;kk++){
													int nTmp=0;
													if (k<2&&v2==0&&v3==k+1||f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v3-(k+1)][kk]>0)
														nTmp=f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v3-(k+1)][kk]+(k+1)*dWeight[j]/nAllNum*n[i];
													if (nMax<nTmp) nMax=nTmp;
												}
											}else{
												for (int kk=0;kk<4;kk++){
													int nTmp=0;
													if (f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v3][kk]>0)
														nTmp=f[v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v3][kk]+(k+1)*dWeight[j]/nAllNum*n[i];
													if (nMax<nTmp) nMax=nTmp;
												}
											}
											if (f[v][v2][v3][(k+1)/3]<nMax) f[v][v2][v3][(k+1)/3]=nMax;
									}
								}
							}
						}
					}
				}
			}
			if (j==0) j=3;
			else if (j==4) j=0;
			else if (j!=1) j--;
			else j=4;//j's order is 0 3 2 1 4
		}while (j!=0);
	}
	g_nMaxValue=0;g_nMaxLeft=0;
	//for (int i=0;i<=n[7]/5;i++)//or plus maxab(...,f[v-1])?
	for (int v2=0;v2<41;v2++)
		for (int v3=0;v3<41;v3++)
			for (int kk=0;kk<4;kk++){
				if (g_nMaxValue<f[n[7]/5][v2][v3][kk]) g_nMaxValue=f[n[7]/5][v2][v3][kk];
			}*/
			if (i!=6&&j==0){//[i-1]..[0(i-1)] is different from [i]..[0(i)]!!!we want to use [i]..[0(i)]!!!
				for (int v=n[7]/5;v>0;v--)
					for (int v2=40;v2>-1;v2--)
						for (int v3=40;v3>-1;v3--)
							for (int kk=0;kk<4;kk++){//if k=-1 only this state need to be compared with former f[v][v2][v3][x]
								if (f[i][0][v][v2][0][0]<f[i+1][4][v][v2][v3][kk]) f[i][0][v][v2][0][0]=f[i+1][4][v][v2][v3][kk];
								//if (v3!=0||kk!=0) f[i][0][v][v2][v3][kk]=0;
							}
			}
			for (int v=n[7]/5;v>0;v--){
				if (i==6){
					if (j==0){
						for (int k=0;k<10;k++)//11 selections
							if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0){
								int nTmp=(k+1)*dWeightMap[j][i];
								if (f[i][j][v][k+1][k+1][(k+1)/3]<nTmp) f[i][j][v][k+1][k+1][(k+1)/3]=nTmp;
							}
					}else{
						for (int v2=40;v2>0;v2--){
							for (int kk=0;kk<4;kk++){//if k=-1 only this state need to be compared with former f[v][v2][v2][x]
								if (f[i][j][v][v2][v2][0]<f[i][(j+1)%4][v][v2][v2][kk]) f[i][j][v][v2][v2][0]=f[i][(j+1)%4][v][v2][v2][kk];
								//if (kk!=0) f[i][j][v][v2][v2][kk]=0;
							}
							for (int k=0;k<10;k++){//1+10 selections
								if (j==4&&k>2) break;
								if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0&&
									(j!=4&&v2-(k+1)>=0||
									j==4&&v2>=15+k*10)){
										int nMax=0;
										if (j!=4){
											for (int kk=(k+1)/3;kk<4;kk++){
												int nTmp=0;
												if (k<2&&v2==k+1||f[i][(j+1)%4][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2-(k+1)][v2-(k+1)][kk]>0)//we must ensure the former one==0 exists only under nV2==nV3==0!!!
													nTmp=f[i][(j+1)%4][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2-(k+1)][v2-(k+1)][kk]+(k+1)*dWeightMap[j][i];
												if (nMax<nTmp) nMax=nTmp;
											}
										}else{
											for (int kk=0;kk<4;kk++){
												int nTmp=0;
												if (f[i][(j+1)%4][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v2][kk]>0)
													nTmp=f[i][(j+1)%4][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v2][kk]+(k+1)*dWeightMap[j][i];
												if (nMax<nTmp) nMax=nTmp;
											}
										}
										if (f[i][j][v][v2][v2][(k+1)/3]<nMax) f[i][j][v][v2][v2][(k+1)/3]=nMax;
								}
							}
						}
					}
				}else{//i!=6
					if (j==0){
						for (int v2=40;v2>-1;v2--){
							for (int v3=40;v3>-1;v3--){
								for (int k=0;k<10;k++){//11 selections
									if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0&&
										v3==k+1&&(k<2||v2>=15+((k-2)/3)*10)){
											int nMax=0;
											for (int kk=0;kk<4;kk++){
												int nTmp=0;
												if (k<2&&v2==0||f[i][0][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][0][kk]>0)//[i-1]..[0(i-1)] is different from [i]..[0(i)]!!!here we want to use [i]..[0(i)], but it doesn't mean we can take twice or more! so we cannot miss the order of v!
													nTmp=f[i][0][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][0][kk]+(k+1)*dWeightMap[j][i];
												if (nMax<nTmp) nMax=nTmp;
											}
											if (f[i][0][v][v2][k+1][(k+1)/3]<nMax) f[i][0][v][v2][k+1][(k+1)/3]=nMax;
									}
								}
							}
						}
					}else{
						for (int v2=40;v2>-1;v2--){
							for (int v3=40;v3>-1;v3--){
								for (int kk=0;kk<4;kk++){//if k=-1 only this state need to be compared with former f[v][v2][v3][x]
									if (f[i][j][v][v2][v3][0]<f[i][(j+1)%4][v][v2][v3][kk]) f[i][j][v][v2][v3][0]=f[i][(j+1)%4][v][v2][v3][kk];
									//if (kk!=0) f[i][j][v][v2][v3][kk]=0;
								}
								for (int k=0;k<10;k++){//1+10 selections
									if (j==4&&k>2) break;
									if (v-g_nCostSum[j][k]/nCostCoeff[i]/5>=0&&
										(j!=4&&v3-(k+1)>=0||
										j==4&&v2>=15+k*10&&v3>=nStraBase[i]+k*10)){
											int nMax=0;
											if (j!=4){
												for (int kk=(k+1)/3;kk<4;kk++){
													int nTmp=0;
													if (k<2&&v3==k+1&&v2==0||f[i][(j+1)%4][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v3-(k+1)][kk]>0)
														nTmp=f[i][(j+1)%4][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v3-(k+1)][kk]+(k+1)*dWeightMap[j][i];
													if (nMax<nTmp) nMax=nTmp;
												}
											}else{
												for (int kk=0;kk<4;kk++){
													int nTmp=0;
													if (f[i][(j+1)%4][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v3][kk]>0)
														nTmp=f[i][(j+1)%4][v-g_nCostSum[j][k]/nCostCoeff[i]/5][v2][v3][kk]+(k+1)*dWeightMap[j][i];
													if (nMax<nTmp) nMax=nTmp;
												}
											}
											if (f[i][j][v][v2][v3][(k+1)/3]<nMax) f[i][j][v][v2][v3][(k+1)/3]=nMax;
									}
								}
							}
						}
					}
				}
			}
			if (j==0) j=3;
			else if (j==4) j=0;
			else if (j!=1) j--;
			else j=4;//j's order is 0 3 2 1 4
		}while (j!=0);
	}
	g_nMaxValue=0;g_nMaxLeft=n[7]/5;
	int nMaxV[3]={0};
	for (int v2=0;v2<41;v2++)
		for (int v3=0;v3<41;v3++)
			for (int kk=0;kk<4;kk++){
				if (g_nMaxValue<f[0][4][n[7]/5][v2][v3][kk]){
					g_nMaxValue=f[0][4][n[7]/5][v2][v3][kk];
					nMaxV[0]=v2;nMaxV[1]=v3;nMaxV[2]=kk;
				}
			}
	for (int i=0;i<7;i++){
		int j=4;
		do{
			int nMax=0,nMaxK=0;
			int v2=nMaxV[0],v3=nMaxV[1],kkRef=nMaxV[2],kMax=nMaxV[2]*3+2,kMin=nMaxV[2]*3-1;
			if (j==4){
				if (kMax>=3){
					kMax=3;kMin=2;//it must be 3!
				}else{//here kMax==2
					int nTmp;
					nTmp=(v3-nStraBase[i]+10)/10;
					if (nTmp<kMax) kMax=nTmp;//when j==4, kMax maybe restricted to 0 or 1 by v3
					nTmp=(v2-nStraBase[6]+10)/10;
					if (nTmp<kMax) kMax=nTmp;//when j==4, kMax maybe restricted to 0 or 1 by v2
				}
			}else if (j==0){
				kMax=v3;kMin=v3-1;//it's an accelerator!
			}else{
				if (kMax>10) kMax=10;
			}
			for (int k=kMax;k>kMin;k--){//11 or 4 selections, but we must use nMaxV[2] to restrict it back
				if (j==0){
					int nTmp=0;
					if (i==6){
						if (g_nMaxLeft>=g_nCostSum[j][k-1]/nCostCoeff[i]/5){
							nMaxK=k;break;
						}
					}else{
						if (i==5){
							for (int kk=0;kk<4;kk++){//no restriction for v2 is the only way to go back
								int nTmp=0;
								if (k==0)
									nTmp=f[i+1][4][g_nMaxLeft][v2][v2][kk];
								else if (g_nMaxLeft-g_nCostSum[j][k-1]/nCostCoeff[i]/5>=0)
									nTmp=f[i+1][4][g_nMaxLeft-g_nCostSum[j][k-1]/nCostCoeff[i]/5][v2][v2][kk]+k*dWeightMap[j][i];
								if (nMax<nTmp){
									nMax=nTmp;
									nMaxV[0]=v2;nMaxV[1]=v2;nMaxK=k;nMaxV[2]=kk;
								}
							}
						}else//no restriction for nV3 with i!=5 means nothing to vit
							for (int nV3=0;nV3<41;nV3++)
								for (int kk=0;kk<4;kk++){
									int nTmp=0;
									if (k==0)
										nTmp=f[i+1][4][g_nMaxLeft][v2][nV3][kk];
									else if (g_nMaxLeft-g_nCostSum[j][k-1]/nCostCoeff[i]/5>=0)
										nTmp=f[i+1][4][g_nMaxLeft-g_nCostSum[j][k-1]/nCostCoeff[i]/5][v2][nV3][kk]+k*dWeightMap[j][i];
									if (nMax<nTmp){
										nMax=nTmp;
										nMaxV[0]=v2;nMaxV[1]=nV3;nMaxK=k;nMaxV[2]=kk;
									}
								}
					}
				}else{//j!=0
					int kkMin=kkRef;
					if (j==4) kkMin=0;//restriction 2
					for (int kk=kkMin;kk<4;kk++){
						int nTmp=0,nV2=v2,nV3=v3;
						if (k==0){
							nTmp=f[i][(j+1)%4][g_nMaxLeft][v2][v3][kk];
						}else if (g_nMaxLeft-g_nCostSum[j][k-1]/nCostCoeff[i]/5>=0){
							if (j==4){//we must judge the max{f[i-1]..+w[i]} not max{f[i-1]}!!!
								nTmp=f[i][(j+1)%4][g_nMaxLeft-g_nCostSum[j][k-1]/nCostCoeff[i]/5][v2][v3][kk]+k*dWeightMap[j][i];
							}else{
								if (i!=6){
									nTmp=f[i][(j+1)%4][g_nMaxLeft-g_nCostSum[j][k-1]/nCostCoeff[i]/5][v2][v3-k][kk]+k*dWeightMap[j][i];
									nV3=v3-k;
								}else{
									nTmp=f[i][(j+1)%4][g_nMaxLeft-g_nCostSum[j][k-1]/nCostCoeff[i]/5][v2-k][v3-k][kk]+k*dWeightMap[j][i];
									nV2=v2-k;nV3=v3-k;
								}
							}
						}
						if (nMax<nTmp){
							nMax=nTmp;
							nMaxV[0]=nV2;nMaxV[1]=nV3;nMaxK=k;nMaxV[2]=kk;
						}
					}
				}
			}
			if (nMaxK>0)
				g_nMaxLeft-=g_nCostSum[j][nMaxK-1]/nCostCoeff[i]/5;
			g_nMaxSet[i][j]=nMaxK;
			if (j==4) j=1;
			else if (j!=0) j=(j+1)%4;
			else j=4;//4 1 2 3 0 4
		}while (j!=4);
	}

	const string strShow[5]={"vit","atk","def","rec","spec"};
	/*cout<<"The result next step value form: "<<endl;
	for (int j=0;j<4;j++){
		cout<<strShow[j]<<": ";
		for (int i=0;i<7;i++){
			cout<<dValue[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<strShow[4]<<": ";
	for (int i=0;i<7;i++){
		cout<<dValue[i][4]<<" ";
	}
	cout<<endl;
	cout<<"The result set form: "<<endl;
	for (int j=0;j<4;j++){
		cout<<strShow[j]<<": ";
		for (int i=0;i<7;i++){
			cout<<nSet[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<strShow[4]<<": ";
	for (int i=0;i<7;i++){
		cout<<nSet[i][4]<<" ";
	}
	cout<<endl<<"Left point: "<<nLeft<<endl;*/
	cout<<"The result set form: "<<endl;
	for (int j=0;j<4;j++){
		cout<<strShow[j]<<": ";
		for (int i=0;i<7;i++){
			cout<<g_nMaxSet[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<strShow[4]<<": ";
	for (int i=0;i<7;i++){
		cout<<g_nMaxSet[i][4]<<" ";
	}
	cout<<endl<<"Left point: "<<g_nMaxLeft*5<<endl;
	cout<<endl<<"Max value: "<<g_nMaxValue<<endl;

	if (ch1!='\n') cin.get();
	cin.get();
	return 0;
}
