%% Make sure you compiled the wrapper. Check the readme file

%read some image
imdata = imread('ngc6543a.jpg');

%must convert to gray scale and double since right now the function only accepts these modalities.

img = double(rgb2gray(imdata));

%show the original image
figure(1); imshow(img,[]);

%invoke the logmap function 
[log,fov]=logmap(img);

%show the logpolar image
figure(2); imshow(log,[]);