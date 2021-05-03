data=importdata('../cmake-build-debug/compareRNGs.tsv','\t',1);
figure()
loglog(data.data(:,1),abs(data.data(:,2)),'x')
hold on
loglog(data.data(:,1),abs(data.data(:,4)),'x')
xlim([1 1E8])
title('Genauigkeit vs n')
xlabel('Anzahl Zufallszahlen n')
ylabel('Abweichung zu \pi')
legend('LCG','MT19937')
 
figure()
loglog(data.data(:,1),data.data(:,3)*1e-6,'x')
hold on
loglog(data.data(:,1),data.data(:,5)*1e-6,'x')
title('Laufzeit vs n')
xlabel('Anzahl Zufallszahlen n')
ylabel('Zeit für n Zufallszahlen in s')
legend('LCG','MT19937',"location",'southeast')


figure()
semilogx(data.data(:,1),data.data(:,2),'x')
hold on
semilogx(data.data(:,1),data.data(:,4),'x')
mean(data.data(:,4)+pi)-pi
length(data.data(:,4))

