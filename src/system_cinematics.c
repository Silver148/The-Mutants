/*
THE MUTANT'S

Authors: Abel Ferrer(aka The_Light) and Juan Yaguaro(aka silverhacker)
File: system_cinematics.c

THIS CODE WILL REMAIN CONFIDENTIAL UNTIL THE PROJECT IS COMPLETED. 
Anyone who leaks this code will be automatically kicked out of the 
group and will be considered a real gay.

Copyright 2025
*/

/*SISTEMA DE CINEMÁTICAS HECHO POR JUAN YAGUARO :D*/

#include "system_cinematics.h"
extern SDL_Window* window;

void InitSystemCinematics()
{
    // Initialize FFmpeg libraries
    avformat_network_init();
}

int PlayCinematic(const char* filepath, SDL_Renderer* renderer)
{
        AVFormatContext *fmt_ctx = NULL;
        int ret = 0;
        if (avformat_open_input(&fmt_ctx, filepath, NULL, NULL) < 0) return -1;
        if (avformat_find_stream_info(fmt_ctx, NULL) < 0) { avformat_close_input(&fmt_ctx); return -1; }

        int video_stream = -1;
        for (unsigned i = 0; i < fmt_ctx->nb_streams; ++i)
            if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) { video_stream = i; break; }
        if (video_stream == -1) { avformat_close_input(&fmt_ctx); return -1; }

        AVCodecParameters *codecpar = fmt_ctx->streams[video_stream]->codecpar;
        const AVCodec *codec = avcodec_find_decoder(codecpar->codec_id);
        if (!codec) { avformat_close_input(&fmt_ctx); return -1; }

        AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
        if (!codec_ctx) { avformat_close_input(&fmt_ctx); return -1; }
        if (avcodec_parameters_to_context(codec_ctx, codecpar) < 0) { ret = -1; goto cleanup; }
        if (avcodec_open2(codec_ctx, codec, NULL) < 0) { ret = -1; goto cleanup; }

        AVFrame *frame = av_frame_alloc();

        int width = codec_ctx->width;
        int height = codec_ctx->height;

        SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (!texture) { ret = -1; goto cleanup; }

        AVPacket *pkt = av_packet_alloc();

        AVRational fr = av_guess_frame_rate(fmt_ctx, fmt_ctx->streams[video_stream], NULL);
        int delay_ms = (fr.num && fr.den) ? (int)(1000.0 * fr.den / fr.num) : 40;
        
        double time_base = av_q2d(fmt_ctx->streams[video_stream]->time_base);
        Uint32 start_time_ms = SDL_GetTicks();
        int quit = 0;
        bool draw = false;
        while (!quit && av_read_frame(fmt_ctx, pkt) >= 0) {
            if (pkt->stream_index == video_stream) {
                if (avcodec_send_packet(codec_ctx, pkt) == 0) {
                    while (avcodec_receive_frame(codec_ctx, frame) == 0) {

                        double pts = frame->best_effort_timestamp * time_base;
                        Uint32 target_ms = (Uint32)(pts * 1000.0);

                        Uint32 elapsed_ms = SDL_GetTicks() - start_time_ms;

                        /*Wait until target time*/
                        while (elapsed_ms < target_ms) {
                            SDL_Event ev;
                            while (SDL_PollEvent(&ev)) { //Handle events
                                if (ev.type == SDL_QUIT) quit = true;
                                switch(ev.type) {
                                    case SDL_KEYDOWN:
                                        if (ev.key.keysym.sym == SDLK_ESCAPE) quit = 1;
                                        break;
                                }
                            }
                    
                            elapsed_ms = SDL_GetTicks() - start_time_ms;
                    
                            if (target_ms - elapsed_ms > 1) {
                                SDL_Delay(1); //Yield CPU
                            }
                        }

                        SDL_UpdateYUVTexture(texture, NULL, 
                                            frame->data[0], frame->linesize[0], // Plano Y
                                            frame->data[1], frame->linesize[1], // Plano U
                                            frame->data[2], frame->linesize[2]  // Plano V
                                            );

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, texture, NULL, NULL);
                        SDL_RenderPresent(renderer);
                    }
                }
            }
            av_packet_unref(pkt);
        }

        goto cleanup;

    cleanup:
        if (texture) SDL_DestroyTexture(texture);
        if (frame) av_frame_free(&frame);
        if (codec_ctx) { avcodec_free_context(&codec_ctx); }
        if (fmt_ctx) avformat_close_input(&fmt_ctx);
        if (pkt) av_packet_free(&pkt);

        return ret;
}

int ShutdownCinematicsSystem()
{
    // Cleanup FFmpeg libraries
    avformat_network_deinit();

    return 0;
}