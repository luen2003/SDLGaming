#include "Common_Function.h"
#include "MainObject.h"
#include "BaseObject.h"
#include "AmoObject.h"
#include "ThreatObject.h"
#include "ExplosionObject.h"
#include "PLayerHeart.h"
#include "TextObject.h"
#include <ctime>


#undef main
TTF_Font* g_font_text = NULL;
TTF_Font* g_font_menu = NULL;

bool Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        return false;
    }

    g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if (g_screen == NULL)
        return false;
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return false;
    // Read file wav audio
    g_sound_bullet[0] = Mix_LoadWAV(g_name_audio_bullet_main);
    g_sound_bullet[1] = Mix_LoadWAV(g_name_audio_bullet_main2);
    g_sound_exp[0] = Mix_LoadWAV(g_name_audio_exp_main);
    g_sound_exp[1] = Mix_LoadWAV(g_name_audio_exp_threats);
    if (g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_exp[0] == NULL || g_sound_exp[1] == NULL)
        return false;
    if (TTF_Init() == -1) {
        return false;
    }
    g_font_text = TTF_OpenFont("Raleway-Medium.ttf", 20);
    g_font_menu = TTF_OpenFont("Raleway-Bold.ttf", 40);
    if (g_font_text == NULL || g_font_menu == NULL)
    {
        return false;
    }

    return true;
}



    
int main(int arc, char* argv[])
{   
    srand(time(NULL));

    int bkgn_x = 0;
    bool is_run_screen = true;
    bool is_quit = false;
    if (Init() == false)
        return 0;

    // Apply Background
    g_bkground = SDLCommonFunc::LoadImage(g_name_bk_ground);
    if (g_bkground == NULL)
    {
        return 0;
    }
    // Make play heart
    PlayerHeart player_heart;
    // Make text
    TextObject score_game;
    TextObject time_game;
    time_game.SetColor(TextObject::RED_TEXT);
    score_game.SetColor(TextObject::RED_TEXT);
    player_heart.Init();
    // Make MainObject
    MainObject main_object;
    main_object.SetRect(POS_X_START_MAIN_OBJ, POS_Y_START_MAIN_OBJ);
    bool ret = main_object.LoadImg(g_name_plane);
    if (!ret) return 0;
    // Init explosion object 
    ExplosionObject exp_main;
    ret = exp_main.LoadImg(g_name_exp_main);
    exp_main.set_clip();
    if (!ret) return 0;
    // Make ThreatObject
    ExplosionObject exp_threats;
    ret = exp_threats.LoadImg(g_name_exp_threats);
    exp_threats.set_clip();
    if (!ret) return 0;
    ThreatObject* p_threats = new ThreatObject[NUM_THREATS];
    for (int i = 0; i < NUM_THREATS; i++)
    {
        ThreatObject* p_threat = (p_threats + i);
        if (p_threat)
        {
            ret = p_threat->LoadImg(g_name_threats);
            if (!ret) return 0;
            int rect_y = rand() % VAL_OFFSET_START_POST_THREAT;
            if (rect_y > SCREEN_HEIGHT - UNDER_LIMIT_THREAT)
            {
                rect_y = SCREEN_HEIGHT / 3;
            }

            p_threat->SetRect(SCREEN_WIDTH + i * VAL_OFFSET_START_POST_THREAT, rect_y);
            p_threat->set_x_val(5);
            AmoObject* p_amo = new AmoObject();
            p_threat->InitAmo(p_amo);
        }

    }

    if (!ret)
    {
        return 0;
    }
    unsigned int die_number = 0;
    unsigned int score_val = 0;
    // Menu
    int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_menu);
    if (ret_menu == 1)
        is_quit = true;

    while (!is_quit)
    {
        while (SDL_PollEvent(&g_even))
        {
            if (g_even.type == SDL_QUIT)
            {
                is_quit = true;
                break;
            }
            main_object.HandleInputAction(g_even, g_sound_bullet);
        }
        //Run Main Object
        if (is_run_screen == true)
        {
            bkgn_x -= 2;
            if (bkgn_x <= -(WIDTH_BACKGROUND - SCREEN_WIDTH))
            {
                is_run_screen = false;
            }
            else
            {
                SDLCommonFunc::ApplySurface(g_bkground, g_screen, bkgn_x, 0);
            }

        }
        else
        {
            SDLCommonFunc::ApplySurface(g_bkground, g_screen, bkgn_x, 0);
        }
        //Show player heart
        player_heart.Render(g_screen);
        //Run main
        main_object.HandleMove();
        main_object.Show(g_screen);
        main_object.MakeAmo(g_screen);


        // Run Threat
        for (int tt = 0; tt < NUM_THREATS; tt++)
        {
            ThreatObject* p_threat = (p_threats + tt);
            if (p_threat)
                p_threat->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
            p_threat->Show(g_screen);
            p_threat->MakeAmo(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);

            // COLISSION THREAT BULLET -> MAIN OBJECT
            bool is_col1 = false;
            std::vector<AmoObject*> amo_arr = p_threat->GetAmoList();

            for (int am = 0; am < (int)amo_arr.size(); am++)
            {
                AmoObject* p_amo = amo_arr.at(am);
                if (p_amo != NULL)
                {
                    is_col1 = SDLCommonFunc::CheckCollision(main_object.GetRect(), p_amo->GetRect());
                    if (is_col1 == true)
                    {
                        for (int tx = 0; tx < 4; tx++)
                        {
                            int x_pos = int(p_amo->GetRect().x - EXP_WIDTH * 0.5);
                            int y_pos = int(p_amo->GetRect().y - EXP_HEIGHT * 0.5);
                            exp_threats.set_frame(tx);
                            exp_threats.SetRect(x_pos, y_pos);
                            exp_threats.ShowEx(g_screen);
                            if (SDL_Flip(g_screen) == -1)
                                return 0;
                        }
                        Mix_PlayChannel(-1, g_sound_exp[0], 0);
                        die_number++;
                        if (die_number <= 2)
                        {
                            SDL_Delay(500);
                            main_object.SetRect(0, SCREEN_HEIGHT / 2);
                            player_heart.Decrease();
                            player_heart.Render(g_screen);
                            if (SDL_Flip(g_screen) == -1) {
                                delete[] p_threats;
                                SDLCommonFunc::CleanUp();
                                SDL_Quit();
                                return 0;
                            }

                        }
                        else {
                            if (MessageBox(NULL, L"GameOver!\nYour HP = 0 !", L"Infomation", MB_ICONINFORMATION) == IDOK)
                            {
                                delete[] p_threats;
                                SDLCommonFunc::CleanUp();
                                SDL_Quit();

                                return 1;
                            }
                        }

                    }

                }
            }
            //Check collision between main and threats
            bool is_col = SDLCommonFunc::CheckCollision(main_object.GetRect(), p_threat->GetRect());
            if (is_col == true)
            {
                for (int ex = 0; ex < 4; ex++) {
                    int x_pos = int((main_object.GetRect().x + main_object.GetRect().w * 0.5) - EXP_WIDTH * 0.5);
                    int y_pos = int((main_object.GetRect().y + main_object.GetRect().h * 0.5) - EXP_HEIGHT * 0.5);
                    exp_main.set_frame(ex);
                    exp_main.SetRect(x_pos, y_pos);
                    exp_main.ShowEx(g_screen);
                    SDL_Delay(50);
                    if (SDL_Flip(g_screen) == -1) {
                        delete[] p_threats;
                        SDLCommonFunc::CleanUp();
                        SDL_Quit();
                        return 0;
                    }
                        
                }
                Mix_PlayChannel(-1, g_sound_exp[0], 0);

                die_number++;
                if (die_number <= 2)
                {
                    SDL_Delay(500);
                    main_object.SetRect(0, SCREEN_HEIGHT / 2);
                    player_heart.Decrease();
                    player_heart.Render(g_screen);
                    if (SDL_Flip(g_screen) == -1) {
                        delete[] p_threats;
                        SDLCommonFunc::CleanUp();
                        SDL_Quit();
                        return 0;
                    }

                }
                else {
                    if (MessageBox(NULL, L"GameOver!\nYour HP = 0 ! ", L"Infomation", MB_ICONINFORMATION) == IDOK)
                    {
                        delete[] p_threats;
                        SDLCommonFunc::CleanUp();
                        SDL_Quit();

                        return 1;
                    }
                }
            }
            // Check collision between main amo and threat
            std::vector<AmoObject*> amo_list = main_object.GetAmoList();
            for (int ii = 0; ii < (int)amo_list.size(); ii++)
            {
                AmoObject* p_amo = amo_list.at(ii);
                if (p_amo != NULL)
                {
                    bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p_threat->GetRect());
                    if (ret_col == true)
                    {
                        score_val++;
                        for (int tx = 0; tx < 4; tx++)
                        {
                            int x_pos = int(p_amo->GetRect().x - EXP_WIDTH * 0.5);
                            int y_pos = int(p_amo->GetRect().y - EXP_HEIGHT * 0.5);
                            exp_threats.set_frame(tx);
                            exp_threats.SetRect(x_pos, y_pos);
                            exp_threats.ShowEx(g_screen);
                            if (SDL_Flip(g_screen) == -1)
                                return 0;



                        }
                        p_threat->Reset(SCREEN_WIDTH + tt * VAL_OFFSET_START_POST_THREAT);
                        main_object.RemoveAmo(ii);
                        Mix_PlayChannel(-1, g_sound_exp[1], 0);
                    }
                }
            }
        }
        //Show time for game
        std::string str_time = "Time: ";
        Uint32 time_val = 31 - SDL_GetTicks() / 1000;
        std::string str_val = std::to_string(time_val);
        str_time += str_val;
        time_game.SetText(str_time);
        time_game.SetRect(SCREEN_WIDTH - 200, 10);
        time_game.CreatFontText(g_font_text, g_screen);
        if (time_val == -1)
        {
            
            if (MessageBox(NULL, L"GameOver!\nTime Over!\n ", L"Infomation", MB_ICONINFORMATION) == IDOK)
            {
                delete[] p_threats;
                SDLCommonFunc::CleanUp();
                SDL_Quit();

                return 1;
            }
        }
        //Show score to screen
        std::string score_val_str = std::to_string(score_val);
        std::string strscore("Score: ");
        strscore += score_val_str;
        score_game.SetText(strscore);
        score_game.CreatFontText(g_font_text, g_screen);
        //Update Screen
        if (SDL_Flip(g_screen) == -1)
            return 0;
    }
    delete[] p_threats;

    SDLCommonFunc::CleanUp();
    SDL_Quit();

    return 1;
}
