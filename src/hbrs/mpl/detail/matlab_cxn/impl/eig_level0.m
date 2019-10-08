% Eigenvalues and eigenvectors
%
% Ready for code generation using MATLAB Coder.
%
% References:
%  https://de.mathworks.com/help/matlab/ref/eig.html

% NOTE:
% Native MATLAB may use different algorithms for eig() than MATLAB Coder.
% If input matrix is not Hermitian, then MATLAB Coder uses the QZ algorithm.
% The results from "[V,D] = eig(A)" in MATLAB Coder will be similar to
% "[V,D] = eig(A,eye(size(A)),'qz')", except that MATLAB Coder normalizes the
% columns of V. Consequently, V may represent a different basis of eigenvectors,
% and the eigenvalues in D may not be in the same order as in MATLAB.
%
% For eig(A) and if A is not Hermitian, MATLAB Coder uses LAPACK's xGEEV.
% For eig(A,B), MATLAB Coder uses LAPACK's xGGEV.
%
% Ref.:
%  R2018b/toolbox/eml/lib/matlab/matfun/eig.m
%  https://stackoverflow.com/a/26810521/6490710

function [eigenvalues, eigenvectors] = eig_level0(A)
    coder.varsize('A', 'x');
    
    % create copies to keep original data constant / untouched
    x = A;
    
    [eigenvectors, eigenvalues] = eig(x, 'vector');
end
