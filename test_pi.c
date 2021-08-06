// #include <math.h>
// // double pow(double n, double p)

// int power(int x, int exp, int n) 
// { 
//     int res = 1;      // Initialize result 
  
//     x = x % n;  // Update x if it is more than or  
//                 // equal to p 
  
//     while (exp > 0) 
//     { 
//         // If y is odd, multiply x with result 
//         if (exp & 1) 
//             res = (res*x) % n; 
  
//         // y must be even now 
//         exp = exp>>1; // y = y/2 
//         x = (x*x) % n;   
//     } 
//     return res; 
// }

// double sigma(double n, int j)
// {
//     double r = 0;
//     double add;
//     int i;
//     double denom = j;
//     double intg;

//     for (i = 0; i < n; i++)
//     {
//         r += (modf((double)power(16, n - i, denom) / denom, &intg));
//         // printf("xxxx %lf\n", r);
//         denom += 8;
//     }

//     i = n + 1;
//     double num = 1. / 16.;
//     while ((add = num / denom) > 0.00000001)
//     {
//         // add = pow(16., n - (double)i) / (8. * (double)i + j);
//         r += add;
//         num /= 16.;
//         denom += 8;
//         // printf("add %lf\n", add);
//     }
//     return (modf(r, &intg));
// }

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {

    int fd = open("srcs/blowfish.dat", O_RDONLY);

    char buf[4];

    for (int i = 0; i < 18; i++)
    {
        read(fd, buf, 4);

        printf(",%#x", *((unsigned int*)buf));
    } 
    printf("\n");

    for (int i = 0; i < 4; i++)
    {
    printf("\n");
        for (int j = 0; j < 256; j ++)
        {
            
            read(fd, buf, 4);

            printf(",%#x", *((unsigned int*)buf));
        }
    printf("\n");
    printf("\n");
    printf("\n");
    }
}