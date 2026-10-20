function [path, totalDistance, thermalCost, nodesExpanded] = ...
    localAStar(map, startPoint, goalPoint, distanceWeight, thermalWeight)

% =========================================================
% THERMAL-AWARE A* PATH PLANNER
%
% Inputs:
%   map            : Thermal map matrix
%   startPoint     : [row col]
%   goalPoint      : [row col]
%   distanceWeight : Weight of distance
%   thermalWeight  : Weight of temperature
%
% Outputs:
%   path           : Nx2 matrix [row col]
%   totalDistance  : Total path distance
%   thermalCost    : Accumulated thermal cost
%   nodesExpanded  : Number of expanded nodes
% =========================================================


% =========================================================
% 1. VALIDATE INPUTS
% =========================================================

if isempty(map)
    error('Thermal map is empty.');
end

if ~isnumeric(map)
    error('Thermal map must be numeric.');
end

if numel(startPoint) ~= 2 || numel(goalPoint) ~= 2
    error('Start and goal points must be [row col].');
end

if ~isnumeric(distanceWeight) || ...
   ~isnumeric(thermalWeight)

    error('Weights must be numeric.');
end

if distanceWeight < 0 || thermalWeight < 0
    error('Distance and thermal weights must be non-negative.');
end


% =========================================================
% 2. GET MAP SIZE
% =========================================================

[rows, cols] = size(map);


% =========================================================
% 3. CONVERT START AND GOAL TO INTEGER INDICES
% =========================================================

startRow = round(startPoint(1));
startCol = round(startPoint(2));

goalRow = round(goalPoint(1));
goalCol = round(goalPoint(2));


% =========================================================
% 4. CHECK START AND GOAL BOUNDARIES
% =========================================================

% NOTE: comparisons with NaN always evaluate to false in MATLAB, so a
% NaN/Inf row or column would silently skip the checks below and reach
% map(row,col) with an invalid index. isnan/isinf are checked explicitly
% first so that case is always caught here instead.
if any(isnan([startRow startCol goalRow goalCol])) || ...
   any(isinf([startRow startCol goalRow goalCol])) || ...
   startRow < 1 || startRow > rows || ...
   startCol < 1 || startCol > cols || ...
   goalRow < 1 || goalRow > rows || ...
   goalCol < 1 || goalCol > cols

    error('Start or goal point is outside the thermal map.');
end


% =========================================================
% 5. CHECK VALID THERMAL DATA
% =========================================================

if isnan(map(startRow,startCol)) || ...
   isinf(map(startRow,startCol))

    error('Start point lies on invalid thermal data.');
end

if isnan(map(goalRow,goalCol)) || ...
   isinf(map(goalRow,goalCol))

    error('Goal point lies on invalid thermal data.');
end


% =========================================================
% 6. CHECK IF START = GOAL
% =========================================================

if startRow == goalRow && ...
   startCol == goalCol

    path = [startRow startCol];

    totalDistance = 0;

    thermalCost = map(startRow,startCol);

    nodesExpanded = 1;

    return;
end


% =========================================================
% 7. PREPARE A* DATA STRUCTURES
% =========================================================

gScore = inf(rows, cols);

fScore = inf(rows, cols);

parentRow = zeros(rows, cols);

parentCol = zeros(rows, cols);

openSet = false(rows, cols);

closedSet = false(rows, cols);


% =========================================================
% 8. INITIALIZE START NODE
% =========================================================

gScore(startRow,startCol) = 0;


hStart = heuristic( ...
    startRow, ...
    startCol, ...
    goalRow, ...
    goalCol);


fScore(startRow,startCol) = ...
    distanceWeight * hStart;


openSet(startRow,startCol) = true;


nodesExpanded = 0;


% =========================================================
% 9. DEFINE 8-NEIGHBOR MOVEMENT
% =========================================================

neighbors = [
    -1  0
     1  0
     0 -1
     0  1
    -1 -1
    -1  1
     1 -1
     1  1
];


% =========================================================
% 10. CALCULATE THERMAL RANGE
% =========================================================

validMask = ~isnan(map) & ~isinf(map);

if ~any(validMask(:))
    error('Thermal map contains no valid temperature data.');
end

validTemperatures = map(validMask);

minTemp = min(validTemperatures);

maxTemp = max(validTemperatures);


% =========================================================
% 11. MAIN A* LOOP
% =========================================================

while any(openSet(:))

    % -----------------------------------------------------
    % Find node with minimum fScore
    % -----------------------------------------------------

    openIndices = find(openSet);

    [~, minIndex] = min(fScore(openIndices));

    currentLinearIndex = ...
        openIndices(minIndex);

    [currentRow,currentCol] = ...
        ind2sub( ...
        [rows cols], ...
        currentLinearIndex);


    % -----------------------------------------------------
    % Goal reached
    % -----------------------------------------------------

    if currentRow == goalRow && ...
       currentCol == goalCol

        path = reconstructPath( ...
            parentRow, ...
            parentCol, ...
            startRow, ...
            startCol, ...
            goalRow, ...
            goalCol);


        [totalDistance,thermalCost] = ...
            calculatePathCost( ...
            path, ...
            map);


        return;

    end


    % -----------------------------------------------------
    % Move current node from OPEN to CLOSED
    % -----------------------------------------------------

    openSet(currentRow,currentCol) = false;

    closedSet(currentRow,currentCol) = true;

    nodesExpanded = nodesExpanded + 1;


    % =====================================================
    % 12. EXAMINE NEIGHBORS
    % =====================================================

    for k = 1:size(neighbors,1)

        newRow = ...
            currentRow + neighbors(k,1);

        newCol = ...
            currentCol + neighbors(k,2);


        % -------------------------------------------------
        % Boundary check
        % -------------------------------------------------

        if newRow < 1 || newRow > rows || ...
           newCol < 1 || newCol > cols

            continue;

        end


        % -------------------------------------------------
        % Ignore closed nodes
        % -------------------------------------------------

        if closedSet(newRow,newCol)

            continue;

        end


        % -------------------------------------------------
        % Ignore invalid thermal cells
        % -------------------------------------------------

        if isnan(map(newRow,newCol)) || ...
           isinf(map(newRow,newCol))

            continue;

        end


        % =================================================
        % 13. MOVEMENT DISTANCE
        % =================================================

        if abs(neighbors(k,1)) + ...
           abs(neighbors(k,2)) == 2

            movementDistance = sqrt(2);

        else

            movementDistance = 1;

        end


        % =================================================
        % 14. NORMALIZED THERMAL COST
        % =================================================

        temperature = ...
            map(newRow,newCol);


        if maxTemp > minTemp

            normalizedTemperature = ...
                (temperature - minTemp) / ...
                (maxTemp - minTemp);

        else

            normalizedTemperature = 0;

        end


        % =================================================
        % 15. COMBINED MOVEMENT COST
        % =================================================

        thermalPenalty = ...
            normalizedTemperature;


        movementCost = ...
            distanceWeight * movementDistance + ...
            thermalWeight * thermalPenalty;


        % =================================================
        % 16. TENTATIVE G SCORE
        % =================================================

        tentativeG = ...
            gScore(currentRow,currentCol) + ...
            movementCost;


        % =================================================
        % 17. BETTER PATH FOUND
        % =================================================

        if tentativeG < ...
                gScore(newRow,newCol)


            % Store parent
            parentRow(newRow,newCol) = ...
                currentRow;

            parentCol(newRow,newCol) = ...
                currentCol;


            % Update G score
            gScore(newRow,newCol) = ...
                tentativeG;


            % -------------------------------------------------
            % Heuristic
            % -------------------------------------------------

            h = heuristic( ...
                newRow, ...
                newCol, ...
                goalRow, ...
                goalCol);


            % -------------------------------------------------
            % F score
            % -------------------------------------------------

            fScore(newRow,newCol) = ...
                tentativeG + ...
                distanceWeight * h;


            % Add to OPEN set
            openSet(newRow,newCol) = true;

        end

    end

end


% =========================================================
% 18. NO PATH FOUND
% =========================================================

error('No valid path could be found.');

end



% =========================================================
% HEURISTIC FUNCTION
% =========================================================

function h = heuristic( ...
    row, ...
    col, ...
    goalRow, ...
    goalCol)

h = sqrt( ...
    (row - goalRow)^2 + ...
    (col - goalCol)^2);

end



% =========================================================
% PATH RECONSTRUCTION
% =========================================================

function path = reconstructPath( ...
    parentRow, ...
    parentCol, ...
    startRow, ...
    startCol, ...
    goalRow, ...
    goalCol)


path = [goalRow goalCol];


currentRow = goalRow;
currentCol = goalCol;


while ~( ...
        currentRow == startRow && ...
        currentCol == startCol)


    pRow = ...
        parentRow(currentRow,currentCol);

    pCol = ...
        parentCol(currentRow,currentCol);


    % -----------------------------------------------------
    % Check parent
    % -----------------------------------------------------

    if pRow == 0 || pCol == 0

        error('Path reconstruction failed.');

    end


    % -----------------------------------------------------
    % Add parent to path
    % -----------------------------------------------------

    path = ...
        [pRow pCol; path];


    currentRow = pRow;

    currentCol = pCol;

end

end



% =========================================================
% CALCULATE FINAL PATH COST
% =========================================================

function [totalDistance,thermalCost] = ...
    calculatePathCost(path,map)


totalDistance = 0;

thermalCost = 0;


for i = 2:size(path,1)


    % -----------------------------------------------------
    % Previous point
    % -----------------------------------------------------

    r1 = path(i-1,1);

    c1 = path(i-1,2);


    % -----------------------------------------------------
    % Current point
    % -----------------------------------------------------

    r2 = path(i,1);

    c2 = path(i,2);


    % -----------------------------------------------------
    % Difference
    % -----------------------------------------------------

    dr = r2 - r1;

    dc = c2 - c1;


    % -----------------------------------------------------
    % Distance
    % -----------------------------------------------------

    stepDistance = ...
        sqrt(dr^2 + dc^2);


    totalDistance = ...
        totalDistance + stepDistance;


    % -----------------------------------------------------
    % Thermal cost
    % -----------------------------------------------------

    thermalCost = ...
        thermalCost + map(r2,c2);

end

end
