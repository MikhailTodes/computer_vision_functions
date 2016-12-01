%__HOUGH TRANSFORM__ 
%__Mikhail Todes__
%__EECS 332__
%First the edges are detected
%Then the parameter space is found
%From here the significant intersections are found
%Finally the lines are detected and drawn

input = imread('test.bmp');
ts = 1;
rs = 1;
threshold = 77;

%First find the edges using canny edge detection
edge_img = edge(rgb2gray(input), 'canny');
imshow (edge_img)

[x, y] = find(edge_img); %find the edges coordinates
%t = 0

%**********CREATING THE PARAMETER SPACE******************
tmax = 180-ts; %max in theta matrix
theta = 0:ts:tmax; %create the theta matrix

rmax = sqrt((size(edge_img, 1) ^ 2) + (size(edge_img, 2) ^ 2));%max in rho matrrho = 1:rs:rmax;%Create rho matrix
rho = 1:rs:rmax; %create the rho matrix

total = zeros(length(rho), length(theta));%An array of zeros initially to store votes

for i = 1:numel(x) %Loop through each coordinate that was found and create its polar coord
    for j = 1:length(theta)
        %t = t +1
        tempT = (pi/180)*(theta(j));
        tempR = y(i)*sin(tempT) + x(i)*cos(tempT);
        
        if((tempR >= 1)&&(tempR <= rho(end)))%Check for validity of the temp Rho
            %The vote is where the difference between temp and rho is equal
            %to the min value of the difference
            vote = (find((abs(tempR - rho))==(min(abs(tempR - rho)))));
            for m = 1:length(vote)
                total(vote(m), j) = total(vote(m), j) +1;%increment the corresponding place in total
            end
        end
    end
end
%********************************************************

total_thresholded = (total - threshold);%apply the threshold

MostSignificant = imregionalmax(total);%Find the maximums
[tempR, tempT] = find(MostSignificant);

%Initialise the newrho and newtheta matrices
newrho = zeros(size(tempR));
newtheta = zeros(size(tempT));

for i = 1:numel(tempR)%Loop though all the elements
    if(total_thresholded(tempR(i), tempT(i)) >= 0)%If the thresholded value is greater than 0
        %Set the final rho and theta 
        newrho(i) = tempR(i);
        newtheta(i) = tempT(i);
    end
end

%Get rid of zeros
newrho = newrho(any(newrho,2),:);
newtheta = newtheta(any(newtheta,2),:);

%Plot Parameter Space
imshow(imadjust(mat2gray(total)), 'XData', newrho, 'YData', newtheta, 'InitialMagnification', 'fit')
 
%plot image with the drawn in lines
imshow(input, 'Border', 'Tight')
hold on;
for i = 1:size(newrho)
    %Finding m, x, and c
    c = (rho(round(newrho(i))) / sind(theta(newtheta(i))));
    m = -(cosd(theta(newtheta(i))) / sind(theta(newtheta(i))));
    x = 1:size(edge_img);
    
         
    %Drawing the lines on the image.
    plot(((m * x) + c), x);
    hold on;
end
hold off;





