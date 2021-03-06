namespace mws
{
        class net_con{
                public:
                        int level;
                        int *num;
        };

        class BP
        {
                public:
                        double *y;
                        double **net;
                        double **o;
                        double **theta;
                        double **delta;
                        double ***w;
                        double ***lw;
                        double eta;
                        double alpha;
                        net_con con;

                        BP(int level,int *num,double inital_eta=1.0,double inital_alpha=0.1,double inital_theta=0.5)
                        {
                                net=new double*[level];
                                o=new double*[level];
                                theta=new double*[level];
                                delta=new double*[level];
                                w=new double**[level-1];
                                lw=new double**[level-1];
                                con.level=level;
                                con.num=num;
                                eta=inital_eta;
                                alpha=inital_alpha;
                                for (int i=0;i<level;i++)
                                {
                                        net[i]=new double[num[i]];
                                        o[i]=new double[num[i]];
                                        theta[i]=new double[num[i]];
                                        delta[i]=new double[num[i]];
                                        for (int j=0;j<num[i];j++)
                                        {
                                                o[i][j]=0;
                                                net[i][j]=0;
                                                theta[i][j]=inital_theta;
                                                delta[i][j]=0;
                                        }
                                }
                                y=o[level-1];
                                for (int i=0;i<(level-1);i++)
                                {
                                        w[i]=new double*[num[i]];
                                        lw[i]=new double*[num[i]];
                                        for (int j=0;j<num[i];j++)
                                        {
                                                w[i][j]=new double[num[i+1]];
                                                lw[i][j]=new double[num[i+1]];
                                                for (int k=0;k<num[i+1];k++)
                                                {
                                                        if ((rand()%2)==0)
                                                        {
                                                                w[i][j][k]=1;
                                                        }else{
                                                                w[i][j][k]=-1;
                                                        }
                                                        lw[i][j][k]=0;
                                                }
                                        }
                                }
                        }

                        double sigmoid(double x)
                        {
                                return (1.0/(1.0+exp(x)));
                        }

                        void calculate_net(double *x)
                        {
                                for (int i=0;i<con.num[0];i++)
                                {
                                        net[0][i]=x[i];
                                        o[0][i]=x[i];
#ifdef _DEBUG_
                                        printf("O[0][%d]=%f\n",i,o[0][i]);
#endif
                                }
                                for (int i=1;i<con.level;i++)
                                {
                                        for (int j=0;j<con.num[i];j++)
                                        {
                                                net[i][j]=0;
                                                for (int k=0;k<con.num[i-1];k++)
                                                {
                                                        net[i][j]+=o[i-1][k]*w[i-1][k][j];
                                                }
                                                o[i][j]=sigmoid(-net[i][j]+theta[i][j]);
#ifdef _DEBUG_
                                                printf("O[%d][%d]=%f\n",i,j,o[i][j]);
#endif
                                        }
                                }
                        }

                        double calculate_delta_and_theta(double *d)
                        {
                                for(int i=(con.level-2);i>=0;i--)
                                {
                                        for (int k=0;k<con.num[i+1];k++)
                                        {
                                                if(i==(con.level-2))
                                                {
                                                        delta[i][k]=(d[k]-y[k])*y[k]*(1-y[k]);
                                                }else{
                                                        double E=0;
                                                        for (int h=0;h<con.num[i+2];h++)
                                                        {
                                                                E+=delta[i+1][h]*w[i+1][k][h];
                                                        }
                                                        delta[i][k]=o[i+1][k]*(1-o[i+1][k])*E;
                                                }
                                                theta[i][k]+=eta*delta[i][k];
                                        }
                                }
                        }

                        void show(double *x)
                        {
                                calculate_net(x);
                                for (int i=0;i<con.num[con.level-1];i++)
                                {
                                        printf("%f,",y[i]);
                                }
                                printf("\n");
                        }

                        void train(double *x,double *d)
                        {
                                calculate_net(x);
                                calculate_delta_and_theta(d);
                                for(int i=(con.level-2);i>=0;i--)
                                {
                                        for (int j=0;j<con.num[i];j++)
                                        {
#ifdef _DEBUG_
                                                printf("num[i]=%dnum[i+1]=%d\n",con.num[i],con.num[i+1]);
#endif
                                                for (int k=0;k<con.num[i+1];k++)
                                                {
                                                        w[i][j][k]+=eta*delta[i][k]*o[i][j]+alpha*lw[i][j][k];
                                                        lw[i][j][k]=eta*delta[i][k]*o[i][j];
#ifdef _DEBUG_
                                                        printf("W[%d][%d][%d]=%f\n",i,j,k,w[i][j][k]);
#endif
                                                }
                                        }
                                }
                        }

                        double E(double *x,double *y)
                        {
                                double e=0;
                                this->calculate_net(x);
                                for (int j=0;j<con.num[con.level-1];j++)
                                {
                                        e+=(this->y[j]-y[j])*(this->y[j]-y[j]);
                                }
                                return sqrt(e);
                        }
        };

        double abs(double x)
        {
                if (x>0)
                {
                        return x;
                }else{
                        return -x;
                }
        }
}
