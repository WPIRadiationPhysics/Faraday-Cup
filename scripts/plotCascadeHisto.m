% Particle cascade field plots
%
% Takes normalized particle post-step (r, z, E_r, E_z) for
% q={e-, p+, o, n, g} data and displays N_q(E_z, E_r) quiver

% Copper dimensions (mm)
Cu_height = 100; Cu_radius = 30;
world_height = Cu_height + 200;
world_radius = Cu_radius*1.5;

for particle=['e' 'p' 'o' 'n' 'g']
  for energy=[70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250]

    % Define cascade file
    cascadeDataFile = strcat('cascadeHistos/', particle, 'Cascade_', num2str(energy), 'MeV.dat');

    % Load data matrix from file
    try % ignores empty files
      cascadeData = load(cascadeDataFile);

      % Parse data and define cascade positions and momenta
      r = cascadeData(:, 1)*world_radius/100; z = cascadeData(:, 2)*world_height/100;
      E_r = cascadeData(:, 3)/energy; E_z = cascadeData(:, 4)/energy;

      % Display momenta on the grid with quiver
      quiver(z, r, E_z, E_r);

      % Format figure
      titleString = strcat(particle, ' Directed Energy Deposition at:   ', num2str(energy), ' MeV');
      title(titleString);
      axis([99 100+Cu_height -1 Cu_radius]); % Centered about copper with 1 mm buffer for outer layers
      xlabel('Z [mm]');
      ylabel('R [mm]');

      % Save in respective folder
      fileName = strcat('cascadeHistos/', particle, 'Cascade_', num2str(energy), 'MeV.png');
      saveas(gcf, fileName)
    
    end
  end
end
