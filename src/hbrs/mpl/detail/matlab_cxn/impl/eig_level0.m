% Eigenvalues and eigenvectors
%
% Ready for code generation using MATLAB Coder.
%
% References:
%  https://de.mathworks.com/help/matlab/ref/eig.html

% NOTE:
% Native MATLAB may use different algorithms for eig() than MATLAB Coder.
% If input matrix is Hermitian (*1) results from "[V,D] = eig(A)" will be 
% similar to "[V,D] = schur(A,'complex')" (*2). Both V and D will be complex.
% If input matrix is not Hermitian, then MATLAB Coder uses the QZ algorithm (*3).
% The results from "[V,D] = eig(A)" in MATLAB Coder will be similar to
% "[V,D] = eig(A,eye(size(A)),'qz')", except that MATLAB Coder normalizes the
% columns of V. Consequently, V may represent a different basis of eigenvectors,
% and the eigenvalues in D may not be in the same order as in MATLAB.
%
% For eig(A) and if A is not Hermitian, MATLAB Coder uses LAPACK's xGEEV (*4). 

% For eig(A) and if A is Hermitian (*1), MATLAB Coder uses schur(), which 
% again uses LAPACK's xGEHRD, xORGHR and xHSEQR functions.
% For eig(A,B), MATLAB Coder uses LAPACK's xGGEV.
%
% (*1) A real matrix is Hermitian if it is symmetric.
% (*2) A matrix must be square, but not necessarily Hermitian to compute the
%      (Generalized) Schur decomposition [4][5].
% (*3) The generalized Schur decomposition is also called QZ decomposition [6,p.375]
% (*4) According to MATLAB Coder's documentation eig() uses the QZ algorithm for
%      non-Hermitian matrices [3]. Its implementation for non-Hermitian matrices
%      then solely uses LAPACK's xGEEV function. Combining both facts that means
%      that LAPACK's xGEEV implements (a variant) of the QZ algorithm.
%
% Ref.:
% [1] R2018b/toolbox/eml/lib/matlab/matfun/eig.m
% [2] https://stackoverflow.com/a/26810521/6490710
% [3] https://de.mathworks.com/help/matlab/ref/eig.html
% [4] https://en.wikipedia.org/wiki/Schur_decomposition
% [5] http://mathworld.wolfram.com/SchurDecomposition.html
% [6] Matrix Computations by Gene Howard Golub and Charles Francis Van Loan (1996) [3rd ed.]

function [eigenvalues, eigenvectors] = eig_level0(A)
    coder.varsize('A', 'x');
    
    % create copies to keep original data constant / untouched
    x = A;
    
    [eigenvectors, eigenvalues] = eig(x, 'vector');
end
