using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace EsbStudio
{
	public class app
	{
        public const int V8_VER_MIN_ = 9;
		public const int V8_VER_MAX_ = 23;

        public static string config_ver_v8(int i_ver_) {
            Debug.Assert(i_ver_ >= app.V8_VER_MIN_ && i_ver_ <= app.V8_VER_MAX_);
            return "v83" + ((i_ver_ < 10) ? "0" + i_ver_.ToString() : i_ver_.ToString());
        }
        public static string config_ver_param(string s_ver_v8_)     { return $"$ESB_CONFIG_{s_ver_v8_}$"; }
        public static string config_ver_param(int i_ver_)           { return config_ver_param(config_ver_v8(i_ver_)); }
        public static string config_run_param(string v8VerTag_)     { return $"$ESB_CONFIG_RUN_{v8VerTag_}$"; }
        public static string config_run_param(int i_ver_)           { return config_run_param(config_ver_v8(i_ver_)); }
        public static string config_cfg_param(string v8VerTag_)     { return $"$ESB_CONFIG_CFG_{v8VerTag_}$"; }
        public static string config_cfg_param(int i_ver_)           { return config_cfg_param(config_ver_v8(i_ver_)); }
        public static string config_sht_param(string v8VerTag_)     { return $"$ESB_CONFIG_SHT_{v8VerTag_}$"; }
        public static string config_sht_param(int i_ver_)           { return config_sht_param(config_ver_v8(i_ver_)); }


    }
}
