function test_watershed_cuts()
build_watershed_cuts()
%close all

%keyboard

demo_image1()
demo_image2()

demo_3d()

fuzzing()

benchmark_2d(2.^(4:12), 1)
benchmark_3d(2.^(7:10))

end

function fuzzing()
% Just check so it does not crash
for kk = 1:100
   ndim = randi(2)+1;
   sz = 1 + randi(20, [1, ndim]);
   I = randn(sz);
   W = watershed_cuts(I);   
end

watershed_cuts(nan(1));
watershed_cuts(nan(2,2));
watershed_cuts(nan(2,2,2));

% Should give an error if more than 3D
gotError = 0;
try
    watershed_cuts(ones(2,2,2,2))
catch error
    gotError = 1;
end
assert(gotError);

% Should give an error if the wrong input type
gotError = 0;
try
    watershed_cuts(single(ones(2,2,2)))
catch error
    gotError = 1;
end
assert(gotError);

% etc ...

end

function benchmark_2d(N, simpleImage)
disp('--> benchmark_2d')
if(simpleImage)
    disp('Using simple=constant image')
else
    disp('Using random image')
end
disp('Iter, watershed [s], watershed_cuts [s]')
T = [];


for kk = 1:numel(N)
    n = N(kk);
    
    if simpleImage == 1
        I = ones(n, n);
    else
        rng(1)
        I = 256*rand(n, n);
        %I = gsmooth(I, 2, 'normalize');
    end
    
    t1 = -1;
    if n < 32768
    tic
    w1 = watershed(I);
    t1 = toc;
    end
    tic
    for kk = 1:10
    w2 = watershed_cuts(I);
    end
    t2=toc/10;
    T = cat(1, T, [n, t1, t2]);
    fprintf('%d, %f, %f\n', n, t1, t2);
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
 
end

function benchmark_3d(N, simpleImage)
disp('--> benchmark_3d')
for simpleImage = 0:1
if(simpleImage)
    disp('Using simple=constant image')
else
    disp('Using random image')
end

T = [];

for kk = 1:numel(N)
    n = N(kk);
    if simpleImage
        I = ones(n, n, n);
    else
        rng(1)
        I = 256*rand(n, n, n);
    end
    t1 = -1;

    tic
    w1 = watershed(I);
    t1 = toc;

    tic
    w2 = watershed_cuts(I);
    t2=toc;
    T = cat(1, T, [n, t1, t2]);
    fprintf('it%d, MATLAB %f, WSC %f\n', n, t1, t2);
end

figure,
subplot(1,2,1)
plot(T(:,1), T(:,2))
hold on
plot(T(:,1), T(:,3));
legend({'watershed', 'watershed cuts'})
xlabel('side length [pixels]');
ylabel('time [s]')
title('3D')
subplot(1,2,2)
plot(T(:,1), T(:,2)./T(:,3))
xlabel('side length [pixels]');
ylabel('Speedup')
title('3D')
end

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
w2 = int32(I);
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

L = watershed(D);
L2 = watershed_cuts(double(D));

L(~bw) = 0;
L2(~bw) = 0;

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

function demo_3d()

center1 = -10;
center2 = -center1;
dist = sqrt(3*(2*center1)^2);
radius = dist/2 * 1.4;
lims = [floor(center1-1.2*radius) ceil(center2+1.2*radius)];
[x,y,z] = meshgrid(lims(1):lims(2));
bw1 = sqrt((x-center1).^2 + (y-center1).^2 + ...
           (z-center1).^2) <= radius;
bw2 = sqrt((x-center2).^2 + (y-center2).^2 + ...
           (z-center2).^2) <= radius;
bw = bw1 | bw2;
figure, 
subplot(1,3,1)
isosurface(x,y,z,bw,0.5), axis equal, title('BW')
xlabel x, ylabel y, zlabel z
xlim(lims), ylim(lims), zlim(lims)
view(3), camlight, lighting gouraud
D = bwdist(~bw);
subplot(1,3,2)
isosurface(x,y,z,D,radius/2), axis equal
title('Isosurface of distance transform')
xlabel x, ylabel y, zlabel z
xlim(lims), ylim(lims), zlim(lims)
view(3), camlight, lighting gouraud
D = -D;
D(~bw) = Inf;

L = watershed_cuts(double(D));

%L0 = watershed(double(D));


L(~bw) = 0;

subplot(1,3,3)
p1 = patch(isosurface(x,y,z,L==1,0.5));
p1.FaceColor = 'red';
p1.EdgeColor = 'none';

p2 = patch(isosurface(x,y,z,L==2,0.5));
p2.FaceColor = 'green';
p2.EdgeColor = 'none';

axis equal
title('Segmented objects')
xlabel x, ylabel y, zlabel z
xlim(lims), ylim(lims), zlim(lims)
view(3), camlight, lighting gouraud

end