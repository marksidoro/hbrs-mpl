% Principal component analysis (PCA)
%
% Uses Singular value decomposition (SVD) algorithm.
% Ready for code generation using MATLAB Coder.
% Function is equal to pca_level1 but weights have been stripped.
%
% References:
%  ${Matlab_ROOT_DIR}/toolbox/stats/stats/pca.m

function [coeff,score,latent,mu] = pca_level2(A, Economy)
    coder.varsize('A', 'x');
    
    x = A; % make copy of A in order to keep A constant / untouched
    
    [m,n] = size(x);
    
    DOF=m-1;
    
    mu = wnanmean(x, ones(1,m,'like',x));
	
    %mu = mean(x);
    x = bsxfun(@minus,x,mu);
    
    if Economy
        [U,S,coeff] = svd(x, 'econ');
    else
        [U,S,coeff] = svd(x, 0);
    end
    
    S = diag(S);
    score =  bsxfun(@times,U,S');
    % these two lines are equal to: score =  U*S;
    
    latent = S.^2./DOF;
    
    if DOF < n
        if Economy
            % When 'Economy' value is true, nothing corresponding to zero
            % eigenvalues should be returned.
            coeff(:, DOF+1:end) = [];
            score(:, DOF+1:end) = [];
            latent(DOF+1:end, :) = [];
        else
            % otherwise, eigenvalues and corresponding outputs need to pad
            % zeros because svd(x,0) does not return columns of U corresponding
            % to components of (DOF+1):p.
            
            %score(:, DOF+1:n) = 0;
            % NOTE: MATLAB Coder generates bounds-violating C code for the line 
            %       above so we have to workaroud:
            score_ = zeros(size(score));
            score_(:,0:DOF) = score(:,0:DOF);
            score = score_;

            %latent(DOF+1:n, 1) = 0;
            latent_ = zeros(size(latent));
            latent_(0:DOF, 1) = latent(0:DOF, 1);
            latent = latent_;
        end
    end
    
    % Enforce a sign convention on the coefficients -- the largest element in
    % each column will have a positive sign.
    [~,maxind] = max(abs(coeff), [], 1);
    [d1, d2] = size(coeff);
    colsign = sign(coeff(maxind + (0:d1:(d2-1)*d1)));
    coeff = bsxfun(@times, coeff, colsign);
    score = bsxfun(@times, score, colsign);
end
