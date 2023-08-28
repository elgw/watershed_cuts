build_watershed_cuts()

center1 = -40;
center2 = -center1;
dist = sqrt(2*(2*center1)^2);
radius = dist/2 * 1.4;
lims = [floor(center1-1.2*radius) ceil(center2+1.2*radius)];
[x,y] = meshgrid(lims(1):lims(2));
bw1 = sqrt((x-center1).^2 + (y-center1).^2) <= radius;
bw2 = sqrt((x-center2).^2 + (y-center2).^2) <= radius;
bw = bw1 | bw2;
imshow(bw)
title('Binary Image with Overlapping Objects')

D = bwdist(~bw);
imshow(D,[])
title('Distance Transform of Binary Image')


D = -D;
imshow(D,[])
title('Complement of Distance Transform')


L = watershed(D);
L2 = watershed_cuts(double(D));

L(~bw) = 0;

figure, 
subplot(2,2,1)
imagesc(D)
title('input')
axis image

subplot(2,2,3)
imagesc(L)
axis image
title('watershed')
subplot(2,2,4)
imagesc(L2)
axis image
title('watershed cuts')

