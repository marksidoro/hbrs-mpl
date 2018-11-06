% Householder Bidiagonalization of matrix A, where A=U*B*V' with U and V being orthogonal
% and B having elements on the main diagonal and first upper diagonal.
% 
% Based on
%  - Bidiag (v1.0.0.0) by Paul Godfrey, <pgodfrey@intersil.com>, Intersil Corp., 8-12-2002 [1]
%  - Algorithm 5.4.2 (Householder Bidiagonalization) [2, page 284ff.]
%
% References:
% [1] https://de.mathworks.com/matlabcentral/fileexchange/3568-bidiag
% [2] Gene Howard Golub and Charles Francis Van Loan (2013): Matrix Computations [4th ed.]

function [UU,b,VV] = bidiag_level0(A, mode)
    coder.varsize('A', 'a');
    
    assert(mode == decompose_mode.complete); % no other mode supported yet

    a = A; % make copy of A in order to keep A constant / untouched

    [r,c]=size(a);
    n=min(r,c);
    UU=eye(r);
    VV=eye(c);

    for k=1:n
        %zero a col
        if k<=r
            x=a(k:end,k);
            v=x;

            %what if x is a zero vector or has x(1)=0?
            if x(1)==0,
                x(1)=eps*eps;
            end
            v(1)=v(1)+sign(x(1))*norm(x);

            U=eye(length(v))-2/(v'*v)*(v*v');
            z=zeros(k-1,r-k+1);
            U=[eye(k-1) z ;
                z.'    U];
            
            UU=U*UU;
            a=U*a;
        end

        %zero a row
        if k<c
            x=a(k,k+1:end);
            v=x.';

            %what if x is a zero vector or has x(1)=0?
            if x(1)==0,
                x(1)=eps*eps;
            end
            v(1)=v(1)+sign(x(1))*norm(x);

            V=conj(eye(length(v))-2/(v'*v)*(v*v'));
            z=zeros(k,c-k);
            V=[eye(k) z ;
                z.'  V];
            
            VV=VV*V;
            a=a*V;
        end

    end

    UU=UU';

    q=sign(diag(a));
    if r==1,q=q(1:1,1:1);end
    if c==1,q=q(1:1,1:1);end
    q(q>=0)=1;
    q=diag(q);

    if r<=c
        UU=UU*q;
        a=q*a;
    else
        VV=q*VV;
        a=a*q;
    end

    b=triu(tril((a),1));

    if nargout<=1
        UU=b;
    end
end
