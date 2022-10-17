n_rows = 3;
n_cols = 5;

F = zeros(n_rows,n_cols);

for i = 1 : n_rows

    for j = 1 : n_cols

        F(i,j) = sqrt(1+i/j);

    end

end

F;

F( : , 2:3) = [];
F


