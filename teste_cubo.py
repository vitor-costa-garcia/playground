# Example file showing a basic pygame "game loop"
import pygame
import numpy as np
from math import sin, cos, sqrt, pi

# pygame setup
pygame.init()
screen = pygame.display.set_mode((1280, 720))
clock = pygame.time.Clock()
running = True

            #1            2             3             4         5              6                 7              8 
cube_v = [[0, 0, 0], [100, 0, 0], [100, 100, 0], [0, 100, 0], [0, 0, 100], [100, 0, 100], [100, 100, 100], [0, 100, 100]]
cube_lines = [[0, 1], [0, 4], [0, 3], [2, 1], [2, 3], [2, 6], [5, 4], [5, 6], [5, 1], [7, 4], [7, 6], [7, 3]]
cube_faces = [[0, 1, 5, 4], [0, 3, 7, 4], [0, 1, 2, 3], [6, 2, 1, 5], [6, 2, 3, 7], [6, 5, 4, 7]]
sx, sy = 1, 1
proj_matrix = np.matrix([[sx, 0, 0], [0, 0, sy]])
offset_matrix = np.matrix([[0],[0]])
scaling_factor = 1

theta_x = 0
theta_y = 0
theta_z = 0

def sphere_func(radius_squared, step):
    points = []
    for i in np.arange(-radius_squared, radius_squared, step):
        for j in np.arange(-radius_squared, radius_squared, step):
            try:
                points.append([i, j, sqrt(radius_squared - i**2 - j**2)])
            except:
                continue

    for i in np.arange(-radius_squared, radius_squared, step):
        for j in np.arange(-radius_squared, radius_squared, step):
            try:
                points.append([i, j, -sqrt(radius_squared - i**2 - j**2)])
            except:
                continue

    for i in np.arange(-radius_squared, radius_squared, step):
        for j in np.arange(-radius_squared, radius_squared, step):
            try:
                points.append([i, sqrt(radius_squared - i**2 - j**2), j])
            except:
                continue

    for i in np.arange(-radius_squared, radius_squared, step):
        for j in np.arange(-radius_squared, radius_squared, step):
            try:
                points.append([i, -sqrt(radius_squared - i**2 - j**2), j])
            except:
                continue

    for i in np.arange(-radius_squared, radius_squared, step):
        for j in np.arange(-radius_squared, radius_squared, step):
            try:
                points.append([sqrt(radius_squared - i**2 - j**2), j, i])
            except:
                continue

    for i in np.arange(-radius_squared, radius_squared, step):
        for j in np.arange(-radius_squared, radius_squared, step):
            try:
                points.append([-sqrt(radius_squared - i**2 - j**2), j, i])
            except:
                continue

    return points


sphere_points = sphere_func(9, 1)

geometry = 'sphere'

while running:
    rot_mX = np.matrix([[1,            0,             0],
                        [0, cos(theta_x), -sin(theta_x)],
                        [0, sin(theta_x), cos(theta_x)]])

    rot_mY = np.matrix([[cos(theta_y),  0, sin(theta_y)],
                        [0,             1,            0],
                        [-sin(theta_y), 0, cos(theta_y)]])

    rot_mZ = np.matrix([[cos(theta_z), -sin(theta_z), 0],
                        [sin(theta_z),  cos(theta_z), 0],
                        [0,              0,           1]])
    
    final_rotation_m = rot_mX*rot_mY*rot_mZ
    # poll for events
    # pygame.QUIT event means the user clicked X to close your window
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    keys = pygame.key.get_pressed()
    if keys[pygame.K_e]:
        theta_x += pi/72
    if keys[pygame.K_d]: #0.14
        theta_x -= pi/72
    if keys[pygame.K_r]:
        theta_y += pi/72
    if keys[pygame.K_f]: #0.14
        theta_y -= pi/72
    if keys[pygame.K_t]:
        theta_z += pi/72
    if keys[pygame.K_g]: #0.14
        theta_z -= pi/72
    if keys[pygame.K_UP]:
        offset_matrix[1][0] -= 3
    if keys[pygame.K_DOWN]:
        offset_matrix[1][0] += 3
    if keys[pygame.K_LEFT]:
        offset_matrix[0][0] -= 3
    if keys[pygame.K_RIGHT]:
        offset_matrix[0][0] += 3
    if keys[pygame.K_w]:
        scaling_factor += 0.1
    if keys[pygame.K_s]: #0.14
        scaling_factor -= 0.1
            
    # fill the screen with a color to wipe away anything from last frame
    screen.fill("#111630")

    # RENDER YOUR GAME HERE
    if geometry == 'cube':
        cube_points_2d = []
        for coord in cube_v:
            posx, posy = scaling_factor*(proj_matrix * (final_rotation_m * np.matrix(coord).transpose())) + offset_matrix
            cube_points_2d.append((posx.item(), posy.item()))
            #pygame.draw.circle(screen, "white", (posx.item(), posy.item()), 5)

        for p1, p2, p3, p4 in cube_faces:
            pygame.draw.polygon(screen, color='grey', points=[cube_points_2d[p1], cube_points_2d[p2], cube_points_2d[p3], cube_points_2d[p4]])

        for start_point, end_point in cube_lines:
            pygame.draw.line(screen, 'white', cube_points_2d[start_point], cube_points_2d[end_point])

    if geometry == 'sphere':
        sphere_points_2d = []
        for coord in sphere_points:
            posx, posy = scaling_factor*(proj_matrix * (final_rotation_m * np.matrix(coord).transpose())) + offset_matrix
            pygame.draw.circle(screen, "white", (posx.item(), posy.item()), 5)
            sphere_points_2d.append((posx.item(), posy.item()))


    # flip() the display to put your work on screen
    pygame.display.flip()

    clock.tick(60)  # limits FPS to 60

pygame.quit()