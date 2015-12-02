% Gain Error profile 2D histogram plot
%
% Displays Error_Gain(z, r) histogram for multilayer analysis

% Copper dimensions (mm)
Cu_height = 100; Cu_radius = 30;
energies = [70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250];

% Create output directory if necessary
system('mkdir -p csv/figs');

for runID=0:6

  % Define profile file
  gainProfileDataFile = strcat('csv/gainErrorProfileCu-', int2str(runID), '.csv');

  % Load data matrix from file
  try % ignores empty files
    gainProfileData = load(gainProfileDataFile);

    % Plot matrix as 2D histogram
    imagesc(gainProfileData);
    colorbar;

    % Format figure
    titleString = strcat('Cu log10(Gain Relative Error) Profile at:   ', num2str(energies(runID+1)), ' MeV');
    title(titleString);
    set(gca, 'XTickLabel', [0 20 40 60 80 100]);
    set(gca, 'YTickLabel', [30 25 20 15 10 5 0]);
    xlabel('Z [mm]');
    ylabel('R [mm]');

    % Save in respective folder
    fileName = strcat('csv/figs/gainErrorProfileCu-', int2str(runID), '.png');
    saveas(gcf, fileName)
    
  end
end
