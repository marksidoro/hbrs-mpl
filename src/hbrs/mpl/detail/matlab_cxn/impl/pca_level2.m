% Principal component analysis (PCA)
%
% Uses Singular value decomposition (SVD) algorithm.
% Ready for code generation using MATLAB Coder.
% Function is equal to pca_level1 but weights have been stripped.
%
% References:
%  ${Matlab_ROOT_DIR}/toolbox/stats/stats/pca.m

function [coeff,score,latent,mu] = pca_level2(A, Economy, Center)
    coder.varsize('A', 'x');
    
    x = A; % make copy of A in order to keep A constant / untouched
    
    [m,n] = size(x);
    
    DOF=m-Center;
    
    if Center
        % NOTE: If MATLAB cannot find wnanmean, then you have to add its parent folder your search path: 
        %       ${Matlab_ROOT_DIR}/toolbox/stats/classreg/+classreg/+learning/+internal/
        mu = wnanmean(x, ones(1,m,'like',x));
        %mu = mean(x);
    else
        mu = zeros(1,n,'like',x);
    end
    
    if Center
        x = bsxfun(@minus,x,mu);
    end
    
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
            % NOTE: this increases the size of score from m*m to m*n if m<n
            %       and Economy=false
            % NOTE: MATLAB Coder generates bounds-violating C code for the
            %       line above so we have to workaroud:
            score_ = zeros([m,n]);
            score_(:,1:DOF) = score(:,1:DOF);
            score = score_;

            %latent(DOF+1:n, 1) = 0;
            % NOTE: this increases the size of latent from min(m,n) to n if m<n
            %       and Economy=false
            latent_ = zeros([n,1]);
            latent_(1:DOF, 1) = latent(1:DOF, 1);
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
