% Dynamic mode decomposition (DMD)
%
% Ready for code generation using MATLAB Coder.
%
% References:
%  Dynamic Mode Decomposition: Data-Driven Modeling of Complex Systems by J. Nathan Kutz et al. (2016)
%  Modal Analysis of Fluid Flows: An Overview by Kunihiko Taira et al. (2017)

function [eigenvalues, modes, coefficients] = dmd_level1(X1, X2, TargetRank)
    coder.varsize('X1', 'X2', 'y1', 'y2');
    
    % create copies to keep original data constant / untouched
    y1 = X1;
    y2 = X2;

    [m,n] = size(y1);
    r = min(TargetRank, min(m,n));
    
    [U, S, V] = svd(y1, 'econ');
    
    U_r = U(:, 1:r);
    S_r = S(1: r, 1:r);
    V_r = V(:, 1:r);
    
    % TODO: Replace transpose with ctranspose for complex numbers
    Atilde = transpose(U_r) * y2 * V_r / S_r;
    
    [W_r , D] = eig(Atilde);
    modes = y2 * V_r / S_r * W_r;
    eigenvalues = diag (D);
    
    col1 = y1(:, 1);
    coefficients = modes \ col1;
end
