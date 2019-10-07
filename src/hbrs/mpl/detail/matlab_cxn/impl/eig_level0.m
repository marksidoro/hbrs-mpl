% Eigenvalues and eigenvectors
%
% Ready for code generation using MATLAB Coder.
%
% References:
%  https://de.mathworks.com/help/matlab/ref/eig.html

function [eigenvalues, eigenvectors] = eig_level0(A)
    coder.varsize('A', 'x');
    
    % create copies to keep original data constant / untouched
    x = A;
    
    [eigenvectors, eigenvalues] = eig(x, 'vector');
end
