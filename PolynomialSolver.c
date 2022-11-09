/* 
** Final Project: Polynomial Equation Solver
** Program By: Mariano Arce
** All Rights Reserved
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define S 100
#define C 0.0001
#define L 0.05

/* Linked List structure */
typedef struct node_s{
    double lower;
    double upper;
    struct node_s *p_next;
} node_t;

void instructions(void);
void getString(char dst[S]);
int validateString(char dst[S]);
void initializeArray(double arr[11]);
void buildArray(char string[S], double arr[11]);
double findCoefficient(int, char string[S]);
void findDerivative(double coefficients[11],double derivative[11]);
double evaluateFunction(double x,double array[11]);
void add_node_at_end(node_t **pp_head, double lower, double upper);
void getRoots(double mid,double coefficients[11],double derivative[11],node_t *p_head);
int findLimits(node_t **pp_head, double lower, double upper, double coefficients[11]);
int checkLimits(double lower, double upper, double coefficients[11]);
double newtonsMethod(double x,double coefficients[11], double derivative[11]);
double bisectionMethod(double lower, double upper, double coefficients[11]);
double roundd(double x);

int main(void){
    char string[S];
    double coefficients[11];
    double derivative[11];
    double lower;
    double upper;
    double mid;
    int v;

    node_t *p_head = NULL;

    //Getting user input
    instructions();
    getString(string);

    //Validating Input
    v = validateString(string);
    while(v == -1){
        printf("Enter Another Function:\n");
        getString(string);
        v = validateString(string);
    }

    //Ask the user for the boundaries
    printf("\nEnter boundaries to search for roots\n");
    printf("Enter the lower limit: ");
    scanf("%lf",&lower);
    printf("Enter the upper limit: "); 
    scanf("%lf",&upper);

    while(upper<lower){
        printf("Your lower limit is higher than your upper limit\n");
        printf("Enter a new upper limit:");
        scanf("%lf",&upper);
    }

    //Compute the midpoint
    mid = (lower+upper)/2;

    //Building Array
    initializeArray(coefficients);
    buildArray(string,coefficients);

    //Find the derivative of the function
    findDerivative(coefficients,derivative);

    //Finding the limits where there might be a root and entering them into the linked list
    findLimits(&p_head,lower,upper,coefficients); 

    //Obtaining the roots and printing them
    getRoots(mid,coefficients,derivative,p_head);

    printf("\n");

    getchar();
    getchar();
}

void instructions(void){
    //Print instructions
    printf("\nLaunching Polynomial Equation Solver\n\n");
    printf("Enter your function the following way:\n");
    printf("CoefficientxPower+CoefficientxPower+Numberc\n");
    printf("You might find this example useful:\n\n");
    printf("f(x)=x+3.00x2+3324.00x4-5390.00x9-5.00c\n\n");
    printf("Make sure you use two decimals on every coefficient.\n");
    printf("Exponents are input plainly after every lowercase x.\n");
    printf("The maximum coefficient size allowed is 9999.99\n");
    printf("If your equation has any constants, enter a lowercase c after it.\n\n");
    printf("Enter your equation:\n\n");
}

void getString(char dst[S]){
    char ch;
    int i = 0;

    printf("f(x)=");

    //Filling string with user input
    ch = getchar();
    while ((ch!='\n')&&(i<S)){
        dst[i++] = ch;
        ch = getchar();
    }
    dst[i] = '\0';
}

int validateString(char dst[S]){
    int c;
    int i;

    //Traverse through the string
    for(c=0;dst[c]!='\0';c++){
        //Tests every coefficient is in the correct form
        if(dst[c]=='.'){
            //Checks for 2 decimal points
            if((dst[c+3]!='x')&&(dst[c+3]!='c')){
                printf("Incorrect Format, enter 2 decimals for every coefficient and make sure you are using only x and c.\n");
                return(-1);
            //Checks for max coefficient size
            } else if((dst[c-2]!='+')&&(dst[c-2]!='-')&&(dst[c-3]!='+')&&(dst[c-3]!='-')
            &&(dst[c-4]!='+')&&(dst[c-4]!='-')&&(dst[c-5]!='+')&&(dst[c-5]!='-')&&(c>4)){
                printf("Incorrect Format, maximum coefficient size is 9999.99\n");
                return(-1);
            }
        //Checks for decimal point in all coefficients
        } else if((dst[c]=='x')&&(dst[c-1]!='+')&&(dst[c-1]!='-')&&(dst[c-3]!='.')&&(c!=0)){
                printf("Incorrect Format, enter 2 decimals for every coefficient.\n");
                return(-1);
        } else if((dst[c]=='c')&&(dst[c-1]!='+')&&(dst[c-1]!='-')&&(dst[c-3]!='.')&&(c!=0)){
                printf("Incorrect Format, enter 2 decimals for every coefficient.\n");
                return(-1);
        }
        //Checks for x's
        if(dst[c]=='x'){
            i = 1;
        }
    }
    //Shows an error if no x's are found
    if(i!=1){
        printf("Incorrect Format, enter at least one x.\n");
        return(-1);
    }
    return(1);
}

void initializeArray(double arr[11]){
    //Clear the array
    for(int i=0;i<11;i++){
        arr[i]=0;
    }
}

void buildArray(char string[S], double arr[11]){
    char ch;
    int c;

    //Traverse through the string
    for(c=0;string[c]!='\0';c++){
        //Fill up the array with the coefficients of X
        if(string[c]=='x'){
            ch = string[c+1];
            switch(ch){
            case '1':
                arr[0]+=findCoefficient(c-1,string); 
                break; 
            case '9':
                arr[1]+=findCoefficient(c-1,string);
                break;
            case '8':
                arr[2]+=findCoefficient(c-1,string);
                break;
            case '7':
                arr[3]+=findCoefficient(c-1,string);
                break;
            case '6':
                arr[4]+=findCoefficient(c-1,string);
                break;
            case '5':
                arr[5]+=findCoefficient(c-1,string);
                break;
            case '4':
                arr[6]+=findCoefficient(c-1,string);
                break;
            case '3':
                arr[7]+=findCoefficient(c-1,string);
                break;
            case '2':
                arr[8]+=findCoefficient(c-1,string);
                break;
            case '+':
            case '-':
            case '\0':
                arr[9]+=findCoefficient(c-1,string);
                break;
            default:
            printf("Parsing Error, Restart Program\n");
            }
        }
        //Fill in the constant number
        if(string[c]=='c'){
            arr[10]=findCoefficient(c-1,string);
        }
    }
}

double findCoefficient(int count, char string[S]){
    double co = 0;
    int i = 0;
    int c = 0;
 
    //Looping through the coefficient going backwards
    while((string[count]!='+')&&(string[count]!='-')&&(count!=-1)){

        //Checking we're not at the decimal point
        if(string[count]!='.'){
            i = (int)string[count]-48;
        }

        //Adding the value of each digit to the coefficient total (case 2 is the decimal)
        switch(c){
        case 0:
            co += i*0.01;
            break;
        case 1:
            co += i*0.1;
            break;
        case 2:
            break;
        case 3:
            co += i;
            break;
        case 4:
            co += i*10;
            break;
        case 5:
            co += i*100;
            break;
        case 6:
            co += i*1000;
        default:
            printf("Parsing Error, Restart Program\n");
        }
        count-=1;
        c++;
    }

    //Checking the coefficient isn't 0
    if(co==0){
        co=1;
    }

    //Giving the coefficient a sign
    if(string[count]=='-'){
        co=-co;
    }

    return(co);
}

void findDerivative(double coefficients[11],double derivative[11]){
    int c;

    //Fill in the derivative array with derivative coefficients 
    for(c=0;c<11;c++){
        derivative[c+1]=coefficients[c]*(10-c);
    }
}

double evaluateFunction(double x,double array[11]){
    double f = 0;

    //Add up the values of every x term into f
    for(int c=0;c<11;c++){
            f += array[c]*pow(x,(10-c));
    }
    return(f);
}

/* Function By Dr. Juan Carlos Rojas
 * Adds a node at the end of a linked list 
 * If the list is empty, the new node becomes the first node (the head is modified)
 * Otherwise we find the last node in the list, and make it point to the new node
 * If the allocation fails, nothing is added 
 */
void add_node_at_end(node_t **pp_head, double lower, double upper)
{
	node_t *p_node;
	node_t *p_last;
	
	/* Add one node */
	p_node = malloc(sizeof(node_t));
	if (p_node == NULL)
	{
		printf("Failed to allocate memory\n");
		return;
	}
	
	/* Initialize node - Edit by Mariano Arce */
	p_node->lower = lower;
    p_node->upper = upper;
	p_node->p_next = NULL;
	
	/* Was the list empty? */
	if (*pp_head == NULL)
	{
		/* Make the head point to the new node */
		*pp_head = p_node;
	}
	else
	{
		/* Find the last node on the list */
		p_last = *pp_head;
		while (p_last->p_next != NULL)
		{
			p_last = p_last->p_next;
		}

		/* Make the last node point to the new node */
		p_last->p_next = p_node;
	}
}

void getRoots(double mid,double coefficients[11],double derivative[11],node_t *p_head){
    int c = 0;
    double x;
    double f;

    //Starting the traversal of linked list
    node_t *p_node;
	p_node = p_head;

    if(p_node==NULL){

        //Check that f(mid) isn't 0, if it is then its the only root.
        f = evaluateFunction(mid,coefficients);
        if(f==0){
            mid = roundd(mid);
            printf("\nThe only real root found was at x = %.2lf\n",mid);
        } else {
            //Use Newtons Method to find the nearest root
            x = newtonsMethod(mid,coefficients,derivative);
            f = evaluateFunction(x,coefficients);
            if (fabs(f)<C){
                x = roundd(x);
                printf("\nNo real roots were found inside the interval; the nearest root is at x = %.2lf\n",x);
            } else {
                printf("\nThe function has no real roots\n");
            }
        }
    }

    //Traversing linked list solving for every found valid limit
	while((p_node != NULL)&&c<S){
        x = bisectionMethod(p_node->lower,p_node->upper,coefficients);
        x = roundd(x);
        printf("\nRoot at x = %.2lf\n",x);
		p_node = p_node->p_next;
        c++;
	}

}

int findLimits(node_t **pp_head, double lower, double upper, double coefficients[11]){
    double mid;
    int i=0;

    //Looking for shorter limits if the difference between limits is greater than L
    if(fabs(upper-lower)>L){
        mid = (upper+lower)/2;
        i += findLimits(pp_head,mid,upper,coefficients);
        i += findLimits(pp_head,lower,mid,coefficients);
    }

        //Adding valid limits to linked list
        if((i==0)&&(checkLimits(lower,upper,coefficients)==1)){
        add_node_at_end(pp_head,lower,upper);
        return(1);
        }

        //Returning values depending on limits added to linked list
        if(i==0){
            return(0);
        } else {
            return(1);
        }
}

int checkLimits(double lower, double upper, double coefficients[11]){
    double flower;
    double fupper;

    //Evaluate the limits
    flower = evaluateFunction(lower,coefficients);
    fupper = evaluateFunction(upper,coefficients);

    //Check if the limits have the same sign
    if(fupper*flower>0){
        return(-1);
    } else {
        return(1);
    }
}

double newtonsMethod(double x, double coefficients[11], double derivative[11]){
    double f;
    double fprime;

    //Find f(x) and f'(x) at starting point x
    f=evaluateFunction(x,coefficients);
    fprime=evaluateFunction(x,derivative);

    //Calculate the root Approximation
    x-=(f/fprime);

    //Find f(x) with the new x
    f=evaluateFunction(x,coefficients);

    //Evaluate x and recursively call newtonsMethod until x is closer to root by less than Macro C
    if(fabs(f)>C){
        x = newtonsMethod(x,coefficients,derivative);
    }
    return(x);
}

double bisectionMethod(double lower, double upper, double coefficients[11]){
    double mid;
    double fmid;
    double fupper;
    double flower;

    //Find the midpoint between the two limits
    mid=(lower+upper)/2;

    //Calculate the values of f at every given point
    fupper = evaluateFunction(upper,coefficients);
    flower = evaluateFunction(lower,coefficients);
    fmid = evaluateFunction(mid,coefficients);

    //Return limits if f=0 or set new limits where the signs of f are still different
    if(fabs(fupper)<C){
        return(upper);
    } else if(fabs(flower)<C){
        return(lower);
    } else if(fabs(fmid)<C){
        return(mid);
    } else if(fmid*fupper>0){
        upper = mid;
    } else {
        lower = mid;
    }

    //Call the function recursively
    mid = bisectionMethod(lower,upper,coefficients);

    return(mid);
}

double roundd(double x){
    int y;

    //Simple rounding of x to 2 decimal digits
    x = 100*x;
    y = round(x);
    x = (double)y/100;
    return(x);
}
