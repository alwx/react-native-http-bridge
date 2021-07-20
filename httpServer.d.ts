declare module "react-native-http-bridge";

export declare function start(
  port: number,
  serviceName: string,
  callback: (request: {
    requestId: string;
    postData?: {};
    type: string;
    url: string;
  }) => void
): void;

export declare function stop(): void;

export declare function respond(
  requestId: string,
  code: number,
  type: string,
  body: string
): void;
