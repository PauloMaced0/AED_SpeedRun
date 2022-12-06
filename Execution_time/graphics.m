clear;
clc;

S_102620 = load("102620_Solution.txt");
S_103511 = load("103511_Solution.txt");
S_108237 = load("108237_Solution.txt");
hf = figure();
x2 = S_102620(:,1);
y2 = S_102620(:,4);
X2 = [ x2.^2,x2,0*x2+1 ];
w2 = pinv(X2)*y2;
format long
w2
plot_1 = plot(x2,y2,'*b',x2,X2*w2,'LineWidth',2,'b');
hold on

x0 = S_103511(:,1);
y0 = S_103511(:,4);
X0 = [ x0.^2,x0,0*x0+1 ];
w0 = pinv(X0)*y0;
format long
w0
plot_2 = plot(x0,y0,'*g',x0,X0*w0,'LineWidth',2,'g');
hold on

x1 = S_108237(:,1);
y1 = S_108237(:,4);
X1 = [ x1.^2,x1,0*x1+1 ];
w1 = pinv(X1)*y1;
format long
w1
plot_3 = plot(x1,y1,'*r',x1,X1*w1,'LineWidth',2,'r');

title("Performance of all working solutions");
xlabel("n");
ylabel("Execution Time");

legend([plot_1,plot_2,plot_3],"102620-values","102620","103511-values","103511","108237-values","108237")

print(hf, "all_solutions", "-dpdflatexstandalone");
system ("pdflatex all_solutions");
open all_solutions.pdf