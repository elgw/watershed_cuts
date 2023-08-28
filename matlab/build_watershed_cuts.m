function build_watershed_cuts()
mex('../src/watershed_cuts_mex.c', '../src/watershed_cuts.c', ...
    '-output' , 'watershed_cuts', '-v', '-DNDEBUG');
end