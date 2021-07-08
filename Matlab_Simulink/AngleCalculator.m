clc
clear
close all
%                  1         2       3       4     5       6
%grade baza  = [175.6565 124.3437 55.6563 4.3437 295.6563 244.3437]
%radiani_baza =[ 3.0658    2.1702    0.9714    0.0758    5.1602    4.2646];

%%BASE PLATE
P0b = [0; 0];
P1b = [-547.67452518637833; -41.599672833967844];
P2b = [-309.86363605024195; -453.5002154];
P3b = [309.86363605652701; -453.50021540000006];
P4b = [547.67452518323671; -41.599672828524966];
P5b = [237.81088912985169; 495.09988823396793];
P6b = [-237.81088913299445; 495.09988822852637];

Pxpoz =[600 0];
Pxneg =[-600 0];
Pypoz =[0  600];
Pyneg =[0 -600];

raza_baza = norm(P0b-P1b);

Pb = [P0b P1b P2b P3b P4b P5b P6b];
Tags = ["P0" "P1" "P2" "P3" "P4" "P5" "P6"];

figure
plot(Pb(1,:),Pb(2,:),'-b'); hold on
plot([Pb(1,2) Pb(1,7)],[Pb(2,2) Pb(2,7)],'-b')
text(Pb(1,:),Pb(2,:),Tags,'VerticalAlignment','top','HorizontalAlignment','left')

title("Baza platformei")

line([P0b(1) P1b(1)],[P0b(2) P1b(2)],'color','b') % Blue line from P0 to P1
line([P0b(1) P2b(1)],[P0b(2) P2b(2)],'color','b') % Blue line from P0 to P2
line([P0b(1) P3b(1)],[P0b(2) P3b(2)],'color','b') % Blue line from P0 to P3
line([P0b(1) P4b(1)],[P0b(2) P4b(2)],'color','b') % Blue line from P0 to P4
line([P0b(1) P5b(1)],[P0b(2) P5b(2)],'color','b') % Blue line from P0 to P5
line([P0b(1) P6b(1)],[P0b(2) P6b(2)],'color','b') % Blue line from P0 to P6

line([Pxneg(1) Pxpoz(1)],[Pyneg(1) Pypoz(1)],'color','r') %dreapta
line([Pxneg(2) Pxpoz(2)],[Pyneg(2) Pypoz(2)],'color','r') %stanga

differenceBase0_1 = (atan((Pxpoz(2)-P0b(2))/(Pxpoz(1)-P0b(1))) - atan((P1b(2)-P0b(2))/(P1b(1)-P0b(1)))) * 180/pi;
differenceBase0_2 = (atan((Pxpoz(2)-P0b(2))/(Pxpoz(1)-P0b(1))) - atan((P2b(2)-P0b(2))/(P2b(1)-P0b(1)))) * 180/pi;
differenceBase0_3 = (atan((Pxpoz(2)-P0b(2))/(Pxpoz(1)-P0b(1))) - atan((P3b(2)-P0b(2))/(P3b(1)-P0b(1)))) * 180/pi;
differenceBase0_4 = (atan((Pxpoz(2)-P0b(2))/(Pxpoz(1)-P0b(1))) - atan((P4b(2)-P0b(2))/(P4b(1)-P0b(1)))) * 180/pi;
differenceBase0_5 = (atan((Pxpoz(2)-P0b(2))/(Pxpoz(1)-P0b(1))) - atan((P5b(2)-P0b(2))/(P5b(1)-P0b(1)))) * 180/pi;
differenceBase0_6 = (atan((Pxpoz(2)-P0b(2))/(Pxpoz(1)-P0b(1))) - atan((P6b(2)-P0b(2))/(P6b(1)-P0b(1)))) * 180/pi;

differenceBase0_Point = [ differenceBase0_1 differenceBase0_2 differenceBase0_3 differenceBase0_4 differenceBase0_5 differenceBase0_6];

%%PLOT CERC
xCenter = 0;
yCenter = 0;
theta = 0 : 0.01 : 2*pi;
radius = 549.2521;
x = radius * cos(theta) + xCenter;
y = radius * sin(theta) + yCenter;
plot(x, y,'k');
%              1       2           3       4       5       6
%grade_top = [158.8263  141.1736   38.8263   21.1736  326.4791  308.8264
%radiani_top =  [ 2.7720    2.4639    0.6776    0.3695    5.6981    5.3900]

%%TOP PLATE 

P0 = [0 ;0];
P1 = [455.79; 176.54];
P2 = [75; -483];
P3 = [-75; -483];
P4 = [-455.79; 176.54];
P5 = [-380.79; 306.45];
P6 = [380.79; 306.45];
P = [P0 P1 P2 P3 P4 P5 P6];
Tags = ["P0" "P1" "P2" "P3" "P4" "P5" "P6"];
figure

plot(P(1,:),P(2,:),'-b'); hold on
plot([P(1,2) P(1,7)],[P(2,2) P(2,7)],'-b')
text(P(1,:),P(2,:),Tags,'VerticalAlignment','top','HorizontalAlignment','left')
title("Platforma mobilă - nerotită")
hold on

%%PLOT CERC
xCenter = 0;
yCenter = 0;
theta = 0 : 0.01 : 2*pi;
radius = 488;
x = radius * cos(theta) + xCenter;
y = radius * sin(theta) + yCenter;
plot(x, y,'k');

line([P0(1) P1(1)],[P0(2) P1(2)],'color','b') % Blue line from P0 to P1
line([P0(1) P2(1)],[P0(2) P2(2)],'color','b') % Blue line from P0 to P2
line([P0(1) P3(1)],[P0(2) P3(2)],'color','b') % Blue line from P0 to P3
line([P0(1) P4(1)],[P0(2) P4(2)],'color','b') % Blue line from P0 to P4
line([P0(1) P5(1)],[P0(2) P5(2)],'color','b') % Blue line from P0 to P5
line([P0(1) P6(1)],[P0(2) P6(2)],'color','b') % Blue line from P0 to P6

line([Pxneg(1) Pxpoz(1)],[Pyneg(1) Pypoz(1)],'color','r') 
line([Pxneg(2) Pxpoz(2)],[Pyneg(2) Pypoz(2)],'color','r') 

% 180 grade rotit
P0 = [0 ;0];
P4 = [455.79; 176.54];
P3 = [75; -483];
P2 = [-75; -483];
P1 = [-455.79; 176.54];
P6 = [-380.79; 306.45];
P5 = [380.79; 306.45];

Pxpoz =[500 0];
Pxneg =[-500 0];
Pypoz =[0  500];
Pyneg =[0 -500];

raza_top = norm(P0-P1);
P = [P0 P1 P2 P3 P4 P5 P6];
Tags = ["P0" "P1" "P2" "P3" "P4" "P5" "P6"];

figure
plot(P(1,:),P(2,:),'-b'); hold on
plot([P(1,2) P(1,7)],[P(2,2) P(2,7)],'-b')
text(P(1,:),P(2,:),Tags,'VerticalAlignment','top','HorizontalAlignment','left')
title("Platforma mobila - rotita cu 180 grade pe axa Y")

line([P0(1) P1(1)],[P0(2) P1(2)],'color','b') % Blue line from P0 to P1
line([P0(1) P2(1)],[P0(2) P2(2)],'color','b') % Blue line from P0 to P2
line([P0(1) P3(1)],[P0(2) P3(2)],'color','b') % Blue line from P0 to P3
line([P0(1) P4(1)],[P0(2) P4(2)],'color','b') % Blue line from P0 to P4
line([P0(1) P5(1)],[P0(2) P5(2)],'color','b') % Blue line from P0 to P5
line([P0(1) P6(1)],[P0(2) P6(2)],'color','b') % Blue line from P0 to P6

line([Pxneg(1) Pxpoz(1)],[Pyneg(1) Pypoz(1)],'color','r') 
line([Pxneg(2) Pxpoz(2)],[Pyneg(2) Pypoz(2)],'color','r') 

differenceTop0_1 = (atan((Pxpoz(2)-P0(2))/(Pxpoz(1)-P0(1))) - atan((P1(2)-P0(2))/(P1(1)-P0(1)))) * 180/pi;
differenceTop0_2 = (atan((Pxpoz(2)-P0(2))/(Pxpoz(1)-P0(1))) - atan((P2(2)-P0(2))/(P2(1)-P0(1)))) * 180/pi;
differenceTop0_3 = (atan((Pxpoz(2)-P0(2))/(Pxpoz(1)-P0(1))) - atan((P3(2)-P0(2))/(P3(1)-P0(1)))) * 180/pi;
differenceTop0_4 = (atan((Pxpoz(2)-P0(2))/(Pxpoz(1)-P0(1))) - atan((P4(2)-P0(2))/(P4(1)-P0(1)))) * 180/pi;
differenceTop0_5 = (atan((Pxpoz(2)-P0(2))/(Pxpoz(1)-P0(1))) - atan((P5(2)-P0(2))/(P5(1)-P0(1)))) * 180/pi;
differenceTop0_6 = (atan((Pxpoz(2)-P0(2))/(Pxpoz(1)-P0(1))) - atan((P6(2)-P0(2))/(P6(1)-P0(1)))) * 180/pi;

differenceTop0_Point = [ differenceTop0_1 differenceTop0_2 differenceTop0_3 differenceTop0_4 differenceTop0_5 differenceTop0_6];

%%PLOT CERC
xCenter = 0;
yCenter = 0;
theta = 0 : 0.01 : 2*pi;
radius = 488;
x = radius * cos(theta) + xCenter;
y = radius * sin(theta) + yCenter;
plot(x, y,'k');


