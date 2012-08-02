%% Make sure you compiled the wrapper. Check the readme file

%read some image
imdata = imread('ngc6543a.jpg');

%must convert to gray scale and double since right now the function only accepts these modalities.
img = double(rgb2gray(imdata));
[l,c] = size(img);

%show the original image
figure(1); imshow(img,[]);

%create a logpolar sensor to process images of this size
lps = openlps(l,c,64,32,5);

%invoke the logmap function 
[log,fov]=logmap(lps,img);

%show the logpolar image
figure(2); imshow(log,[]);

%destroy the log polar object
closelps(lps);