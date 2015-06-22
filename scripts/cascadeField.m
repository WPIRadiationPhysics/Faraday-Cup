% Particle cascade field plots
%
% Takes normalized particle post-step (r, z, p_r, p_z) for
% q={e-, p+, o, n, g} data and displays N_q(p_z, p_r) quiver

% Define cascade file
cascadeDataFile = "data/model0/cascades/eCascade_250MeV.dat";

% Load data matrix from file
cascadeData = load(cascadeDataFile);

% Parse data and define cascade positions and momenta
r = cascadeData(:, 1); z = cascadeData(:, 2);
p_r = cascadeData(:, 3); p_z = cascadeData(:, 4);

% Display momenta on the grid with quiver
quiver(z, r, p_z, p_r);
