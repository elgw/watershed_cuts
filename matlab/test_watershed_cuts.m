function test_watershed_cuts()
build_watershed_cuts()
close all

demo_image1()
demo_image2()
benchmark()
end

function benchmark()
T = [];
N = 2.^(8:11);

for kk = 1:numel(N)
    n = N(kk);
    I = 256*rand(n, n);
    I = gsmooth(I, 2, 'normalize');
    tic
    w1 = watershed(I);
    t1 = toc;
    tic
    w2 = watershed_cuts(I);
    t2=toc;
    T = cat(1, T, [n, t1, t2]);
end

figure,
subplot(1,2,1)
plot(T(:,1), T(:,2))
hold on
plot(T(:,1), T(:,3));
legend({'watershed', 'watershed cuts'})
xlabel('side length [pixels]');
ylabel('time [s]')
subplot(1,2,2)
plot(T(:,1), T(:,2)./T(:,3))
xlabel('side length [pixels]');
ylabel('Speedup')
keyboard

end

function demo_image2()
% Similar image to what is used in the benchmarking
figure,  
n = 256;
I = rand(n, n);
I = gsmooth(I, 4, 'normalize');
l=subplot(2,2,1); imagesc(I), axis image
title('input')
l(2) = subplot(2,2,2); imagesc(watershed(I)), axis image
title('watershed')
w2=watershed_cuts(I);
l(3) = subplot(2,2,3); imagesc(w2), axis image
title('watershed cuts');
ec = w2-imerode(w2, [0, 1, 0 ; 1 1 1; 0 1 0]);
w2e = w2;
w2e(ec>0) = 0;
l(4) = subplot(2,2,4); imagesc(w2e), axis image
title('watershed cuts (edges=0)')
linkaxes(l)
end

function demo_image1()
% taken from doc watershed

center1 = -40;
center2 = -center1;
dist = sqrt(2*(2*center1)^2);
radius = dist/2 * 1.4;
lims = [floor(center1-1.2*radius) ceil(center2+1.2*radius)];
[x,y] = meshgrid(lims(1):lims(2));
bw1 = sqrt((x-center1).^2 + (y-center1).^2) <= radius;
bw2 = sqrt((x-center2).^2 + (y-center2).^2) <= radius;

bw = bw1 | bw2;
D = bwdist(~bw);
D = -D;

L = watershed(D-1);
L2 = watershed_cuts(double(D));

L(~bw) = 0;

figure, 
subplot(1,3,1)
imagesc(D)
title('input')
axis image
subplot(1,3,2)
imagesc(L)
axis image
title('watershed')
subplot(1,3,3)
imagesc(L2)
axis image
title('watershed cuts')

end