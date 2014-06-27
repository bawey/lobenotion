size(labels);

o1=[];
o2=[];
o1=repmat(-1,size(labels,1),100);

data_row=1;

while(data(data_row,1)<labels(1,1))
	++data_row;
end;

for(label_row=1:size(labels,1))
	stimuli_time=labels(label_row, 1);
	is_last=label_row==size(labels,1);
	o1(label_row, 100)=labels(label_row,2);
	destination_column=1;
	while(data(data_row, 1)>=stimuli_time && !is_last && data(data_row, 1)<labels(label_row+1, 1))
		if(destination_column<100)
			o1(label_row, destination_column)=data(data_row, 14)+data(data_row, 15);
		end;
		++data_row;
		++destination_column;
	end;
end;

%  this is 'hard-scripted' removal of zeros in the last sample
o1=o1(1:size(o1,1)-1,:);


variance = max(max(o1(:,1:99)))-min(min(o1(:,1:99)));
o1_data=o1(:,1:99);
o1_meta=o1(:,100);

o1_data.-=min(min(o1_data));
o1_data./=variance;

pos=o1_data(o1_meta==1, :);
neg=o1_data(o1_meta==0, :);

y=[0:98];
y.*=7.82;

plot(y, mean(neg),'r');
hold on;
plot(y, mean(pos),'g');
hold off;
print -S800,600 /home/bawey/Desktop/p300.png