#include "SystemCreatorScene.h"

#include <iostream>

void SystemCreatorScene::Setup()
{
    sun.mass = 1.989e30;
    sun.radius = 696347055;
    sun.size = 10;
    sun.color = 0xfffff4ea;

    ImGuiIO& io = ImGui::GetIO();
}

void SystemCreatorScene::Destroy()
{
}

void SystemCreatorScene::Input()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        ImGuiIO& io = ImGui::GetIO();

        switch (sdlEvent.type)
        {
        case SDL_QUIT:
        {
            Engine::SetIsRunning(false);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            if (mouseButtonDown)
            {
                systemOffset.x += sdlEvent.motion.xrel * systemSize * 0.0005f;
                systemOffset.z -= sdlEvent.motion.yrel * systemSize * 0.0005f;
            }
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            systemSize -= sdlEvent.wheel.y * systemSize * 0.1f;

            if (systemSize <= 5e9)
            {
                systemSize = 5e9;
                return;
            }
            else if (systemSize >= 262e11)
            {
                systemSize = 260e11;
                return;
            }

            float r = io.DisplaySize.x / 2;
            float t = io.DisplaySize.y / 2;
            float mouseX = (r - io.MousePos.x) / r;
            float mouseZ = ((t - io.MousePos.y) / t) * Graphics::yAspect;

            systemOffset.x += (systemSize / 25) * mouseX * sdlEvent.wheel.y;
            systemOffset.z -= ((systemSize) / 25) * mouseZ * sdlEvent.wheel.y;

            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            switch (sdlEvent.button.button)
            {
            case SDL_BUTTON_LEFT:
            {
                if (!click)
                {
                    if (io.MousePos.x < 500) return;

                    Vec3Double systemSelectionPoint;
                    systemSelectionPoint.x = (io.MousePos.x / io.DisplaySize.x) * systemSize - systemSize / 2 + systemOffset.x * (systemSize / systemSizeDefault);
                    systemSelectionPoint.z = -((io.MousePos.y / io.DisplaySize.y) * systemSize - systemSize / 2) * Graphics::yAspect + systemOffset.z;

                    std::cout << systemSelectionPoint.x << ", " << systemSelectionPoint.z << std::endl;

                    selectedPlanet = nullptr;
                    SelectNearestPlanet(systemSelectionPoint);

                    if (selectedPlanet && selectedPlanet != &sun)
                    {
                        massExponent = selectedPlanet->massExponent;
                        mass = selectedPlanet->mass / pow(10, massExponent);

                        semiMajorExponent = selectedPlanet->semiMajorExponent;
                        semiMajor = selectedPlanet->semiMajorAxis / pow(10, semiMajorExponent);
                        eccentricity = selectedPlanet->eccentricity;
                        argPeriapsis = selectedPlanet->argPeriapsis;
                        trueAnomaly = selectedPlanet->trueAnomaly;
                        trueColor = selectedPlanet->color;
                    }
                }

                mouseButtonDown = true;
                click = true;
                break;
            }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            switch (sdlEvent.button.button)
            {
            case SDL_BUTTON_LEFT:
            {
                mouseButtonDown = false;
                click = false;
                break;
            }
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                Engine::SetIsRunning(false);

            if (sdlEvent.key.keysym.sym == SDLK_COMMA)
            {
                Engine::DecrementTimeAccel();
                break;
            }
            if (sdlEvent.key.keysym.sym == SDLK_PERIOD)
            {
                Engine::IncrementTimeAccel();
                break;
            }
            break;
        }
        }
    }
}

void SystemCreatorScene::Update(float deltaTime)
{
    if (simulateOrbit)
    {
        for (size_t i = 0; i < planets.size(); i++)
        {
            //planets[i].trueAnomaly += M_PI * deltaTime;

            planets[i].currentPeriod += deltaTime * 1000000;
            if (planets[i].currentPeriod >= planets[i].period) planets[i].currentPeriod -= planets[i].period;

            // Find Mean Anomaly
            planets[i].meanAnomaly = planets[i].n * (planets[i].currentPeriod - planets[i].period);

            // Find Eccentric Anomaly
            double E = planets[i].meanAnomaly;
            for (size_t j = 0; j < 10; j++)
            {
                E =
                    E -
                    (E - planets[i].eccentricity * sin(E) - planets[i].meanAnomaly) /
                    (1 - planets[i].eccentricity * cos(E));
            }

            // Find True Anomaly
            planets[i].trueAnomaly =
                2 *
                atan(
                    sqrt((1 + planets[i].eccentricity) / (1 - planets[i].eccentricity)) *
                    tan(E / 2)
                );

            CalculatePlanetPosition(&planets[i]);
        }

        if (selectedPlanet) trueAnomaly = selectedPlanet->trueAnomaly;
    }

    DrawGUI();
}

void SystemCreatorScene::Render()
{
    double min = systemSize / 2;


    double sunU = (min + systemOffset.x) / systemSize;
    double sunV = ((systemOffset.z / Graphics::yAspect) + min) / systemSize;

    int radius = (sun.radius / SunRadius) * 50;

    Graphics::DrawFillCircle(Graphics::screenWidth * sunU, Graphics::screenHeight * sunV, sun.size, sun.color);
    if (&sun == selectedPlanet)
    {
        Graphics::DrawCircle(Graphics::screenWidth * sunU, Graphics::screenHeight * sunV, 15, 0.0f, 0xFFFFAA00);
    }

    for (size_t i = 0; i < planets.size(); i++)
    {
        double planetU = ((planets[i].position.x + systemOffset.x) + min) / systemSize;
        double planetV = ((planets[i].position.z + systemOffset.z) / Graphics::yAspect + min) / systemSize;

        radius = (planets[i].radius / SunRadius) * 50;

        Graphics::DrawFillCircle(Graphics::screenWidth * planetU, Graphics::screenHeight * planetV, 10, planets[i].color);

        DrawOrbit(planets[i]);

        if (&planets[i] == selectedPlanet)
        {
            Graphics::DrawCircle(Graphics::screenWidth * planetU, Graphics::screenHeight * planetV, 15, 0.0f, 0xFFFFAA00);
        }
    }
}

void SystemCreatorScene::DrawGUI()
{
    //ImGui::ShowDemoWindow();

    static bool p;
    ImGui::Begin("System Creator", &p, ImGuiWindowFlags_NoMove);
    ImGui::Text(("System Size: " + std::to_string(systemSize)).c_str());
    ImGui::Text(("X Offset: " + std::to_string(systemOffset.x)).c_str());
    ImGui::Text(("Y Offset: " + std::to_string(systemOffset.y)).c_str());

    // Sun Settings
    ImGui::SeparatorText("Sun");

    if (ImGui::InputFloat("Mass kg 10e30", &sunMass, 0.01f, 1.0f, "%.3f")) SetSunSettings(sunMass);

    if (ImGui::Button("Create Sun"))
    {
        SetSunSettings(sunMass);
    }

    // New Planet Settings
    ImGui::SeparatorText("Planet");

    ImGui::Text("Physical Properties");
    if (ImGui::InputFloat("Mass kg", &mass, 0.01f, 9.9f, "%.3f"))
    {
        if (selectedPlanet)
        {
            selectedPlanet->mass = mass * pow(10, massExponent);

            double u = G * (sun.mass + mass);
            selectedPlanet->period = (2 * M_PI) * sqrt(pow(selectedPlanet->semiMajorAxis, 3) / u);
        }
    }
    if (ImGui::SliderInt("Mass Exponent 10e", &massExponent, 20, 28))
    {
        if (selectedPlanet) selectedPlanet->mass = mass * pow(10, massExponent);
    }

    ImGui::Text("Kepler Orbit Elements");
    if (ImGui::SliderFloat("Semi-Major Axis", &semiMajor, 1.0f, 9.9f))
    {
        if (selectedPlanet)
        {
            selectedPlanet->semiMajorAxis = semiMajor * pow(10, semiMajorExponent);

            double u = G * (sun.mass + selectedPlanet->mass);
            selectedPlanet->period = (2 * M_PI) * sqrt(pow(selectedPlanet->semiMajorAxis, 3) / u);

            selectedPlanet->n = sqrt((G * sun.mass) / pow(selectedPlanet->semiMajorAxis, 3));
            selectedPlanet->currentPeriod = (selectedPlanet->trueAnomaly / (M_PI * 2)) * selectedPlanet->period;

            CalculatePlanetOrbit(selectedPlanet);
        }
    }
    if (ImGui::SliderInt("Semi-Major Exponent", &semiMajorExponent, 10, 15))
    {
        if (selectedPlanet)
        {
            selectedPlanet->semiMajorExponent = semiMajorExponent;
            selectedPlanet->semiMajorAxis = semiMajor * pow(10, semiMajorExponent);

            double u = G * (sun.mass + selectedPlanet->mass);
            selectedPlanet->period = (2 * M_PI) * sqrt(pow(selectedPlanet->semiMajorAxis, 3) / u);

            selectedPlanet->n = sqrt((G * sun.mass) / pow(selectedPlanet->semiMajorAxis, 3));


            CalculatePlanetOrbit(selectedPlanet);
        }
    }
    if (ImGui::SliderFloat("Eccentricity", &eccentricity, 0.0f, 0.7f))
    {
        if (selectedPlanet)
        {
            selectedPlanet->eccentricity = eccentricity;
            CalculatePlanetOrbit(selectedPlanet);
        }
    }
    if (ImGui::SliderFloat("Argument of Perigee", &argPeriapsis, 0.0f, 360.0f))
    {
        if (selectedPlanet)
        {
            selectedPlanet->argPeriapsis = (argPeriapsis / 180.0f) * M_PI;
            CalculatePlanetOrbit(selectedPlanet);
        }
    }
    if (ImGui::SliderFloat("True Anomaly", &trueAnomaly, -3.141592f, 3.141592f))
    {
        if (selectedPlanet)
        {
            selectedPlanet->trueAnomaly = trueAnomaly;

            float anomaly = selectedPlanet->trueAnomaly;
            if (anomaly < 0.0f) anomaly += M_PI * 2;
            else if (anomaly > M_PI) anomaly -= M_PI * 2;

            selectedPlanet->currentPeriod = (anomaly / (M_PI * 2)) * selectedPlanet->period;
            CalculatePlanetOrbit(selectedPlanet);
        }
    }

    // Generate a default palette. The palette will persist and can be edited.
    static bool saved_palette_init = true;
    static ImVec4 saved_palette[32] = {};
    if (saved_palette_init)
    {
        for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
        {
            ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
                saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
            saved_palette[n].w = 1.0f; // Alpha
        }
        saved_palette_init = false;
    }

    static ImVec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    static ImVec4 backup_color;
    bool open_popup = ImGui::ColorButton("MyColor##3b", color);
    ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
    open_popup |= ImGui::Button("Color");
    if (open_popup)
    {
        ImGui::OpenPopup("mypicker");
        backup_color = color;
    }
    if (ImGui::BeginPopup("mypicker"))
    {
        ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
        ImGui::Separator();
        ImGui::ColorPicker4("##picker", (float*)&color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
        ImGui::SameLine();

        ImGui::BeginGroup(); // Lock X position
        ImGui::Text("Current");
        ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
        ImGui::Text("Previous");
        if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
            color = backup_color;
        ImGui::Separator();
        ImGui::Text("Palette");
        for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
        {
            ImGui::PushID(n);
            if ((n % 8) != 0)
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

            ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
            if (ImGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
                color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

            // Allow user to drop colors into each palette entry. Note that ColorButton() is already a
            // drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                    memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                    memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                ImGui::EndDragDropTarget();
            }

            ImGui::PopID();
        }

        uint32_t a = 0xFF000000;
        uint32_t r = 0x00FF0000 * color.x;
        uint32_t g = 0x0000FF00 * color.y;
        uint32_t b = 0x000000FF * color.z;

        trueColor = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

        if (selectedPlanet) selectedPlanet->color = trueColor;

        ImGui::EndGroup();
        ImGui::EndPopup();
    }

    if (ImGui::Button("Add Planet"))
    {
        CelestialBody newPlanet;
        newPlanet.mass = mass * pow(10, massExponent);
        newPlanet.massExponent = massExponent;

        newPlanet.semiMajorAxis = semiMajor * pow(10, semiMajorExponent);
        newPlanet.semiMajorExponent = semiMajorExponent;
        newPlanet.eccentricity = eccentricity;
        newPlanet.argPeriapsis = (argPeriapsis / 180.0f) * M_PI;
        newPlanet.trueAnomaly = (trueAnomaly / 180.0f) * M_PI;
        newPlanet.color = trueColor;

        double u = G * (sun.mass + newPlanet.mass);
        newPlanet.period = (2 * M_PI) * sqrt(pow(newPlanet.semiMajorAxis, 3) / u);

        newPlanet.n = sqrt((G * sun.mass) / pow(newPlanet.semiMajorAxis, 3));

        CalculatePlanetOrbit(&newPlanet);

        planets.push_back(newPlanet);

        selectedPlanet = &planets.back();
    }
    if (ImGui::Button("Remove Planet"))
    {
        if (selectedPlanet)
        {
            for (size_t i = 0; i < planets.size(); i++)
            {
                if (&planets[i] == selectedPlanet)
                {
                    CelestialBody temp = planets.back();
                    planets.back() = *selectedPlanet;
                    planets[i] = temp;

                    planets.pop_back();

                    selectedPlanet = nullptr;
                }
            }
        }
    }

    // Selected Planet Properties
    ImGui::SeparatorText("Selected Planet");
    if (selectedPlanet && selectedPlanet != &sun)
    {
        float truePeriod = selectedPlanet->period / 86400.0f;
        ImGui::Text(("Orbital Period (Days): " + std::to_string(truePeriod)).c_str());
    }

    // Simulation
    ImGui::SeparatorText("Simulation");
    ImGui::Checkbox("Simulate Orbit", &simulateOrbit);

    ImGui::End();
}

void SystemCreatorScene::CalculatePlanetPosition(CelestialBody* planet)
{
    double a;
    double r;

    float x;
    float z;

    r =
        (planet->semiMajorAxis * (1 - pow(planet->eccentricity, 2))) /
        (1 + planet->eccentricity * cos(planet->trueAnomaly));

    planet->position.x =
        r * (cos(planet->ascLongitude) * cos(planet->argPeriapsis + planet->trueAnomaly) -
            sin(planet->ascLongitude) * sin(planet->argPeriapsis + planet->trueAnomaly) * cos(planet->inclination));

    /*planet->position.y =
        r * sin(planet->argPeriapsis + planet->trueAnomaly) * sin(planet->inclination);*/

    planet->position.z =
        r * (sin(planet->ascLongitude) * cos(planet->argPeriapsis + planet->trueAnomaly) +
            cos(planet->ascLongitude) * sin(planet->argPeriapsis + planet->trueAnomaly) * cos(planet->inclination));
}

void SystemCreatorScene::SelectNearestPlanet(const Vec3Double& selectionPoint)
{
    double nearestDistance = std::numeric_limits<double>::max();
    double distance;

    for (size_t i = 0; i < planets.size(); i++)
    {
        distance = (planets[i].position - selectionPoint).Magnitude();
        if (distance > maxSelectionDistance) continue;

        if (distance < nearestDistance)
        {
            nearestDistance = distance;
            selectedPlanet = &planets[i];
        }
    }

    distance = (sun.position - selectionPoint).Magnitude();
    if (distance > maxSelectionDistance) return;

    if (distance < nearestDistance)
    {
        nearestDistance = distance;
        selectedPlanet = &sun;
    }
}

void SystemCreatorScene::SetSunSettings(const float mass)
{
    double trueMass = mass * pow(10, 30);
    sun.radius = SunRadius * pow((trueMass / SunMass), 0.8f);

    float relMass = trueMass / SunMass;

    if (relMass > 16.0f) // O
    {
        sun.color = 0xff9bb0ff;
    }
    else if (relMass > 2.1f) // B
    {
        sun.color = 0xffaabfff;
    }
    else if (relMass > 1.4f) // A
    {
        sun.color = 0xffcad7ff;
    }
    else if (relMass > 1.04f) // F
    {
        sun.color = 0xfff8f7ff;
    }
    else if (relMass > 0.8f) // G
    {
        sun.color = 0xfffff4ea;
    }
    else if (relMass > 0.45f) // K
    {
        sun.color = 0xffffd2a1;
    }
    else if (relMass > 0.08f) // M
    {
        sun.color = 0xffffcc6f;
    }
}

void SystemCreatorScene::CalculatePlanetOrbit(CelestialBody* planet)
{
    double a;
    double r;

    float x;
    float z;

    r =
        (planet->semiMajorAxis * (1 - pow(planet->eccentricity, 2))) /
        (1 + planet->eccentricity * cos(planet->trueAnomaly));

    planet->position.x =
        r * (cos(planet->ascLongitude) * cos(planet->argPeriapsis + planet->trueAnomaly) -
            sin(planet->ascLongitude) * sin(planet->argPeriapsis + planet->trueAnomaly) * cos(planet->inclination));

    /*planet->position.y =
        r * sin(planet->argPeriapsis + planet->trueAnomaly) * sin(planet->inclination);*/

    planet->position.z =
        r * (sin(planet->ascLongitude) * cos(planet->argPeriapsis + planet->trueAnomaly) +
            cos(planet->ascLongitude) * sin(planet->argPeriapsis + planet->trueAnomaly) * cos(planet->inclination));

    for (size_t i = 0; i < 32; i += 1)
    {
        a = (static_cast<float>(i) / 32) * (M_PI * 2);
        r = (planet->semiMajorAxis * (1 - pow(eccentricity, 2))) /
            (1 + eccentricity * cos(a));

        x = r * (cos(planet->ascLongitude) * cos(planet->argPeriapsis + a) -
            sin(planet->ascLongitude) * sin(planet->argPeriapsis + a) * cos(planet->inclination));

        /*position.y =
        r * sin(argPeriapsis + a) * sin(inclination);*/

        z = r * (sin(planet->ascLongitude) * cos(planet->argPeriapsis + a) +
            cos(planet->ascLongitude) * sin(planet->argPeriapsis + a) * cos(planet->inclination));

        planet->orbitPoints[i] = Vec3Double(x, 0.0f, z);
    }
}

void SystemCreatorScene::DrawOrbit(const CelestialBody& planet)
{
    double min = systemSize / 2;
    double xO = -systemOffset.x * systemSize * 0.0005f;
    double zO = -systemOffset.z * systemSize * 0.0005f;

    for (size_t i = 0; i < 32; i++)
    {
        size_t j = i + 1 >= 32 ? 0 : i + 1;

        float u = (planet.orbitPoints[i].x + systemOffset.x + min) / systemSize;
        float v = (((planet.orbitPoints[i].z + systemOffset.z) / Graphics::yAspect) + min) / systemSize;

        float w = (planet.orbitPoints[j].x + systemOffset.x + min) / systemSize;
        float x = (((planet.orbitPoints[j].z + systemOffset.z) / Graphics::yAspect) + min) / systemSize;

        Graphics::DrawLine(Graphics::screenWidth * u, Graphics::screenHeight * v, Graphics::screenWidth * w, Graphics::screenHeight * x, planet.color);
    }
}
