% Particle cascade field plots
%
% Takes normalized particle post-step (r, z, E_r, E_z) for
% q={e-, p+, o, n, g} data and displays N_q(E_z, E_r) quiver

for particle=['e' 'p' 'o' 'n' 'g']
  for energy=[70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250]
    for model_i=0:2

      % Copper dimensions (mm)
      Cu_height = 100; Cu_radius = 30;

      if model_i == 0

        % Remainer of simulation dimensions
        world_height = Cu_height + 200;
        world_radius = Cu_radius*1.5;
  
        % Define cascade file
        cascadeDataFile = strcat('data/model', int2str(model_i), '/cascades/', particle, 'Cascade_', num2str(energy), 'MeV.dat');

        % Load data matrix from file
        try
          cascadeData = load(cascadeDataFile);
        catch
          break;
        end

        % Parse data and define cascade positions and momenta
        r = cascadeData(:, 1)*world_radius/100; z = cascadeData(:, 2)*world_height/100;
        E_r = cascadeData(:, 3)/energy; E_z = cascadeData(:, 4)/energy;

        % Display momenta on the grid with quiver
        quiver(z, r, E_z, E_r);

        % Format figure
        titleString = strcat(particle, ' Directed Energy Deposition in Copper at:   ', num2str(energy), ' MeV');
        title(titleString);
        axis([99 100+Cu_height -1 Cu_radius]);
        xlabel('Z [mm]');
        ylabel('R [mm]');

        % Save in respective folder
        fileName = strcat('data/model', int2str(model_i), '/cascades/', particle, 'Cascade_', num2str(energy), 'MeV.png');
        saveas(gcf, fileName)

      else
        for KA_i=[59 100 200]

          % Remainer of simulation dimensions
          KA_height = Cu_height + 2*(KA_i/1000);
          KA_radius = Cu_radius + (KA_i/1000);
          if model_i == 1
            model_height = KA_height; model_radius = KA_radius;
            world_height = KA_height + 200;
            world_radius = KA_radius*1.5;
          else
            AgKA_height = KA_height + 2*(0.012 + 0.062);
            AgKA_radius = KA_radius + (0.012 + 0.062);
            model_height = AgKA_height; model_radius = AgKA_radius;
            world_height = AgKA_height + 200;
            world_radius = AgKA_radius*1.5;
          end

          % Define cascade file
          cascadeDataFile = strcat('data/model', int2str(model_i), '/S', int2str(KA_i), '/cascades/', particle, 'Cascade_', num2str(energy), 'MeV.dat');

          % Load data matrix from file
          try
            cascadeData = load(cascadeDataFile);
          catch
            break;
          end

          % Parse data and define cascade positions and momenta
          r = cascadeData(:, 1)*world_radius/100; z = cascadeData(:, 2)*world_height/100;
          E_r = cascadeData(:, 3)/energy; E_z = cascadeData(:, 4)/energy;

          % Display momenta on the grid with quiver
          quiver(z, r, E_z, E_r);

          % Format figure
          titleString = strcat(particle, ' Directed Energy Deposition in S', int2str(KA_i), ' at:   ', num2str(energy), ' MeV');
          title(titleString);
          axis([99 100+model_height -1 model_radius]);
          xlabel('Z [mm]');
          ylabel('R [mm]');

          % Save in respective folder
          fileName = strcat('data/model', int2str(model_i), '/S', int2str(KA_i), '/cascades/', particle, 'Cascade_', num2str(energy), 'MeV.png')
          saveas(gcf, fileName)

        end
      end
    end
  end
end
